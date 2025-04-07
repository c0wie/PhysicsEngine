// header
#include "algo.hpp"

// local
#include "collision_body.hpp"
#include "vector2.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>

namespace pe2d::algo {
Vec2f FindCircleBoxContactPoint(const std::array<Vec2f, 4> &box_vertices,
                                Vec2f circle_center) {
  float min_distance_squared = math::INF;
  Vec2f contact_point;
  for (std::size_t i = 0; i < box_vertices.size(); i++) {
    float distance_to_circle_center_squared = 0.0f;
    Vec2f cp;
    const Vec2f p1 = box_vertices[i];
    const Vec2f p2 = box_vertices[(i + 1) % box_vertices.size()];
    PointSegmentDistance(circle_center, p1, p2,
                         distance_to_circle_center_squared, cp);
    if (distance_to_circle_center_squared < min_distance_squared) {
      min_distance_squared = distance_to_circle_center_squared;
      contact_point = cp;
    }
  }
  return contact_point;
}
std::pair<Vec2f, Vec2f>
FindBoxBoxContactPoint(const std::array<Vec2f, 4> &box_verticesA,
                       const std::array<Vec2f, 4> &box_verticesB) {
  constexpr float error = 0.00005f;
  float min_distance_squared = math::INF;
  Vec2f contact_point1, contact_point2;
  unsigned int contact_count = 1;
  for (std::size_t i = 0; i < box_verticesA.size(); i++) {
    for (std::size_t j = 0; j < box_verticesB.size(); j++) {
      float vertexA_edgeB_distance = 0.0f;
      Vec2f contact_point;
      const Vec2f p1 = box_verticesB[j];
      const Vec2f p2 = box_verticesB[(j + 1) % box_verticesB.size()];
      PointSegmentDistance(box_verticesA[i], p1, p2, vertexA_edgeB_distance,
                           contact_point);

      if (math::NearlyEquel(vertexA_edgeB_distance, min_distance_squared,
                            error)) {
        if (!math::NearlyEquel(contact_point, contact_point1, error)) {
          contact_point2 = contact_point;
          contact_count = 2;
        }
      } else if (vertexA_edgeB_distance < min_distance_squared) {
        min_distance_squared = vertexA_edgeB_distance;
        contact_point1 = contact_point;
        contact_count = 1;
      }
    }
  }

  for (std::size_t i = 0; i < box_verticesB.size(); i++) {
    for (std::size_t j = 0; j < box_verticesA.size(); j++) {
      float vertexB_edgeA_distance_squared = 0.0;
      Vec2f contact_point;
      const Vec2f p1 = box_verticesA[j];
      const Vec2f p2 = box_verticesA[(j + 1) % box_verticesA.size()];
      PointSegmentDistance(box_verticesB[i], p1, p2,
                           vertexB_edgeA_distance_squared, contact_point);

      if (math::NearlyEquel(vertexB_edgeA_distance_squared,
                            min_distance_squared, error)) {
        if (!math::NearlyEquel(contact_point, contact_point1, error)) {
          contact_point2 = contact_point;
          contact_count = 2;
        }
      } else if (vertexB_edgeA_distance_squared < min_distance_squared) {
        min_distance_squared = vertexB_edgeA_distance_squared;
        contact_point1 = contact_point;
        contact_count = 1;
      }
    }
  }
  if (contact_count == 2) {
    return {contact_point1, contact_point2};
  }
  return {contact_point1, Vec2f(-1.0, -1.0)};
}
void PointSegmentDistance(Vec2f point, Vec2f vertexA, Vec2f vertexB,
                          float &distance_squared, Vec2f &contact_point) {
  if (distance_squared < 0.0) {
    throw std::invalid_argument(
        "[PointSegmentDistance()] Error: distance_sqaure must be non-negative "
        "(received" +
        std::to_string(distance_squared) + ")");
  }
  const Vec2f ab = vertexB - vertexA;
  const Vec2f ap = point - vertexA;
  const float proj = math::Dot(ap, ab);
  // relative point's projection onto the line segment
  const float d = proj / math::SquaredLength(ab);
  if (d <= 0.0) {
    contact_point = vertexA;
  } else if (d >= 1.0) {
    contact_point = vertexB;
  } else {
    contact_point = vertexA + ab * d;
  }
  distance_squared = math::SquaredDistance(point, contact_point);
}

std::array<Vec2f, 4> GetBoxVertices(Vec2f box_size, Transform transform) {
  if (box_size.x <= 0.0 || box_size.y <= 0.0) {
    throw std::invalid_argument("[FindCircleCircleCollision] Error: box_size "
                                "must be positive (received " +
                                box_size.GetString() + ")");
  }
  const Vec2f center = transform.position;
  const Vector2 scale = transform.scale;
  const float scaled_half_sizeX = (box_size.x * scale.x) / 2.0;
  const float scaled_half_sizeY = (box_size.y * scale.y) / 2.0;
  std::array<Vec2f, 4> vertices = {
      Vec2f(center.x + scaled_half_sizeX, center.y - scaled_half_sizeY),
      Vec2f(center.x - scaled_half_sizeX, center.y - scaled_half_sizeY),
      Vec2f(center.x - scaled_half_sizeX, center.y + scaled_half_sizeY),
      Vec2f(center.x + scaled_half_sizeX, center.y + scaled_half_sizeY)};
  RotateVertices(vertices, center, transform.angle);
  return vertices;
}

std::array<Vec2f, 2> GetBoxAxes(const std::array<Vec2f, 4> &vertices) {
  std::array<Vec2f, 2> axes;
  // is has two parrarel edges so I don't have to check other two
  for (int i = 0; i < 2; i++) {
    const Vec2f vertex1 = vertices[i];
    const Vec2f vertex2 = vertices[(i + 1) % vertices.size()];
    const Vec2f edge = vertex1 - vertex2;
    const Vec2f normal = math::Normalize(math::Perp(edge));
    axes[i] = normal;
  }
  return axes;
}

Vec2f ProjectCircle(Vec2f circle_center, float radius, Vec2f axis) {
  if (radius <= 0) {
    throw std::invalid_argument("[FindCircleCircleCollision] Error: radius "
                                "must be positive (received " +
                                std::to_string(radius) + ")");
  }
  const Vector2 dir = axis * radius;
  const Vec2f p1 = circle_center + dir;
  const Vec2f p2 = circle_center - dir;
  float min = math::Dot(p1, axis);
  float max = math::Dot(p2, axis);
  if (min > max) {
    std::swap(min, max);
  }
  return {min, max};
}

float CalculateRotationalInertia(int type, Vec2f size, float mass) {
  switch (type) {
    case Circle:
      return 0.5 * mass * std::pow(size.x, 2);
    case Box:
      return 0.083 * mass * (std::pow(size.x, 2) + std::pow(size.y, 2));
    default:
      break;
  }
  return 0.0;
}

bool PointInsideRect(Vec2f top_left_corner, Vec2f bottom_right_corner, Vec2f point) {
  return (point.x >= top_left_corner.x && point.x <= bottom_right_corner.x &&
    point.y >= top_left_corner.y && point.y <= bottom_right_corner.y);
}

} // namespace pe2d::algo