#pragma once

// local
#include "angle.hpp"
#include "math.hpp"
#include "transform.hpp"
#include "vector2.hpp"

// std
#include <array>
#include <cmath>

namespace pe2d {
template <typename T> concept isContainer = requires(T c) {
  c.begin();
  c.end();
};

namespace algo {
// Returns information about contact point in a collision beetwen box and circle
Vec2f FindCircleBoxContactPoint(const std::array<Vec2f, 4> &box_vertices,
                                Vec2f circle_center);

// Returns information about contact points in a collision beetwen two boxes
std::pair<Vec2f, Vec2f>
FindBoxBoxContactPoint(const std::array<Vec2f, 4> &box_verticesA,
                       const std::array<Vec2f, 4> &box_verticesB);

// point - point to which distance is calculated
// vertexA, vertexB - vertices forming segment,
// contact point
void PointSegmentDistance(Vec2f point, Vec2f vertexA, Vec2f vertexB,
                          float &distance_squared, Vec2f &contact_point);

void RotateVertices(isContainer auto &vertices, Vec2f center, Angle angle) {
  const float cos_angle = std::cos(angle.AsRadians());
  const float sin_angle = std::sin(angle.AsRadians());
  for (auto &vertex : vertices) {
    const float relativeX = vertex.x - center.x;
    const float relativeY = vertex.y - center.y;

    const float rotatedX = (relativeX * cos_angle) - (relativeY * sin_angle);
    const float rotatedY = (relativeX * sin_angle) + (relativeY * cos_angle);
    vertex = Vec2f(rotatedX + center.x, rotatedY + center.y);
  }
}

// Returns vertices in counterclockwise direction starting at top right box
// corner
std::array<Vec2f, 4> GetBoxVertices(Vec2f box_size, Transform transform);

/*
    Indicates whether overlap occurred between the two projections of two edges
   and gives depth of overlap by modifying value of overlap parameter
*/
constexpr bool Overlap(Vec2f a, Vec2f b, float &overlap) {
  if (a.y >= b.x && b.y >= a.x) {
    const float overlap_start = std::max(a.x, b.x);
    const float overlap_end = std::min(a.y, b.y);
    overlap = overlap_end - overlap_start;
    return true;
  }
  overlap = -1.0f;
  return false;
}

// Returns a collection of two normalized direction vectors representing the
// edges created by the given vertices.
std::array<Vec2f, 2> GetBoxAxes(const std::array<Vec2f, 4> &vertices);

// Returns normalized direction vector representing edge created by closest
// vertex and circle center
Vec2f GetCircleAxis(const isContainer auto &vertices, Vec2f circle_center) {
  float dist = math::INF;
  Vec2f smallestAxis;
  for (const auto &vertex : vertices) {
    const Vec2f edge = circle_center - vertex;
    const float d = math::Length(edge);
    if (d < dist) {
      dist = d;
      smallestAxis = edge;
    }
  }
  return math::Normalize(smallestAxis);
}
// Returns a projection of object, represented by given vertices, onto a
// specified axis
Vec2f Project(const isContainer auto &vertices, Vec2f axis) {
  float min = math::Dot(axis, vertices[0]);
  float max = min;

  for (auto it = std::next(vertices.begin()); it != vertices.end(); it++) {
    const float p = math::Dot(axis, *it);
    if (p < min) {
      min = p;
    } else if (p > max) {
      max = p;
    }
  }
  return {min, max};
}

// Returns a projection of circle, represented by given circle center and
// radius, onto a specified axis
Vec2f ProjectCircle(Vec2f circle_center, float radius, Vec2f axis);

float CalculateRotationalInertia(int type, Vec2f size, float mass);

} // namespace algo
} // namespace pe2d