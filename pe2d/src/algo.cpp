// header
#include "algo.hpp"

// local
#include "vector2.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>

namespace pe2d::algo {
Pos2d FindCircleBoxContactPoint(const std::array<Pos2d, 4> &box_vertices,
                                Pos2d circle_center) {
  double min_distance_squared = math::INF;
  Pos2d contact_point;
  for (std::size_t i = 0; i < box_vertices.size(); i++) {
    double distance_to_circle_center_squared = 0.0f;
    Pos2d cp;
    const Pos2d p1 = box_vertices[i];
    const Pos2d p2 = box_vertices[(i + 1) % box_vertices.size()];
    PointSegmentDistance(circle_center, p1, p2,
                         distance_to_circle_center_squared, cp);
    if (distance_to_circle_center_squared < min_distance_squared) {
      min_distance_squared = distance_to_circle_center_squared;
      contact_point = cp;
    }
  }
  return contact_point;
}
std::pair<Pos2d, Pos2d>
FindBoxBoxContactPoint(const std::array<Pos2d, 4> &box_verticesA,
                       const std::array<Pos2d, 4> &box_verticesB) {
  constexpr double error = 0.00005f;
  double min_distance_squared = math::INF;
  Pos2d contact_point1, contact_point2;
  unsigned int contact_count = 1;
  for (std::size_t i = 0; i < box_verticesA.size(); i++) {
    for (std::size_t j = 0; j < box_verticesB.size(); j++) {
      double distanceFromVertexAToEdgeBSquared = 0.0f;
      Pos2d cp;
      const Pos2d p1 = box_verticesB[j];
      const Pos2d p2 = box_verticesB[(j + 1) % box_verticesB.size()];
      PointSegmentDistance(box_verticesA[i], p1, p2,
                           distanceFromVertexAToEdgeBSquared, cp);
      if (math::NearlyEquel(distanceFromVertexAToEdgeBSquared,
                            min_distance_squared, error)) {
        if (!math::NearlyEquel(cp, contact_point1, error)) {
          contact_point2 = cp;
          contact_count = 2;
        }
      } else if (distanceFromVertexAToEdgeBSquared < min_distance_squared) {
        min_distance_squared = distanceFromVertexAToEdgeBSquared;
        contact_point1 = cp;
        contact_count = 1;
      }
    }
  }
  for (std::size_t i = 0; i < box_verticesB.size(); i++) {
    for (std::size_t j = 0; j < box_verticesA.size(); j++) {
      double vertexB_edgeA_distance_squared = 0.0;
      Pos2d cp;
      const Pos2d p1 = box_verticesA[j];
      const Pos2d p2 = box_verticesA[(j + 1) % box_verticesA.size()];
      PointSegmentDistance(box_verticesB[i], p1, p2,
                           vertexB_edgeA_distance_squared, cp);
      if (math::NearlyEquel(vertexB_edgeA_distance_squared,
                            min_distance_squared, error)) {
        if (!math::NearlyEquel(cp, contact_point1, error)) {
          contact_point2 = cp;
          contact_count = 2;
        }
      } else if (vertexB_edgeA_distance_squared < min_distance_squared) {
        min_distance_squared = vertexB_edgeA_distance_squared;
        contact_point1 = cp;
        contact_count = 1;
      }
    }
  }
  if (contact_count == 2) {
    return {contact_point1, contact_point2};
  }
  return {contact_point1, Pos2d(-1.0, -1.0)};
}
void PointSegmentDistance(Pos2d point, Pos2d vertexA, Pos2d vertexB,
                          double &distance_squared, Pos2d &contact_point) {
  if (distance_squared < 0.0) {
    throw std::invalid_argument(
        "[PointSegmentDistance()] Error: distance_sqaure must be non-negative "
        "(received" +
        std::to_string(distance_squared) + ")");
  }
  const Vec2d ab = vertexB - vertexA;
  const Vec2d ap = point - vertexA;
  const double proj = math::Dot(ap, ab);
  const double ab_length_squared = math::SquaredLength(ab);
  // relative point's projection onto the line segment
  const double d = proj / ab_length_squared;
  if (d <= 0.0f) {
    contact_point = vertexA;
  } else if (d >= 1.0f) {
    contact_point = vertexB;
  } else {
    contact_point = vertexA + ab * d;
  }
  distance_squared = math::SquaredDistance(point, contact_point);
}

std::array<Pos2d, 4> GetBoxVertices(Size2d box_size, Transform transform) {
  if (box_size.x <= 0.0 || box_size.y <= 0.0) {
    throw std::invalid_argument("[FindCircleCircleCollision] Error: box_size "
                                "must be positive (received " +
                                box_size.GetString() + ")");
  }
  const Pos2d center = transform.position;
  const Vector2 scale = transform.scale;
  const double scaled_half_sizeX = (box_size.x * scale.x) / 2.0;
  const double scaled_half_sizeY = (box_size.y * scale.y) / 2.0;
  std::array<Pos2d, 4> vertices = {
      Pos2d(center.x - scaled_half_sizeX, center.y - scaled_half_sizeY),
      Pos2d(center.x + scaled_half_sizeX, center.y - scaled_half_sizeY),
      Pos2d(center.x + scaled_half_sizeX, center.y + scaled_half_sizeY),
      Pos2d(center.x - scaled_half_sizeX, center.y + scaled_half_sizeY)};
  RotateVertices(vertices, center, transform.angle);
  return vertices;
}
std::array<Vec2d, 2> GetBoxAxes(const std::array<Pos2d, 4> &vertices) {
  std::array<Vec2d, 2> axes;
  // is has two parrarel edges so I don't have to check other two
  for (int i = 0; i < 2; i++) {
    const Pos2d p1 = vertices[i];
    const Pos2d p2 = vertices[(i + 1) % vertices.size()];
    const Vec2d edge = p1 - p2;
    const Vec2d normal = math::Normalize(math::Perp(edge));
    axes[i] = normal;
  }
  return axes;
}
Vec2d ProjectCircle(Pos2d circle_center, double radius, Vec2d axis) {
  if (radius <= 0) {
    throw std::invalid_argument("[FindCircleCircleCollision] Error: radius "
                                "must be positive (received " +
                                std::to_string(radius) + ")");
  }
  const Vector2 dir = axis * radius;
  const Pos2d p1 = circle_center + dir;
  const Pos2d p2 = circle_center - dir;
  double min = math::Dot(p1, axis);
  double max = math::Dot(p2, axis);
  if (min > max) {
    std::swap(min, max);
  }
  return {min, max};
}
} // namespace pe2d::algo