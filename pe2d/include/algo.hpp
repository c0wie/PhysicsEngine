#pragma once

// local
#include "angle.hpp"
#include "math.hpp"
#include "transform.hpp"
#include "vector2.hpp"

// std
#include <array>

namespace pe2d {
class CircleCollider;
class BoxCollider;

template <typename T> concept isContainer = requires(T c) {
  c.begin();
  c.end();
};

namespace algo {
// Returns information about contact point in a collision beetwen box and circle
Pos2d FindCircleBoxContactPoint(const std::array<Pos2d, 4> &box_vertices,
                                Pos2d circle_center);

// Returns information about contact points in a collision beetwen two boxes
std::pair<Pos2d, Pos2d>
FindBoxBoxContactPoint(const std::array<Pos2d, 4> &box_verticesA,
                       const std::array<Pos2d, 4> &box_verticesB);

// point - point to which distance is calculated
// vertexA, vertexB - vertices forming segment,
// contact point
void PointSegmentDistance(Pos2d point, Pos2d vertexA, Pos2d vertexB,
                          double &distance_squared, Pos2d &contact_point);

void RotateVertices(isContainer auto &vertices, Pos2d center, Angle angle) {
  const double cos_angle = cosf(angle.AsRadians());
  const double sin_angle = sinf(angle.AsRadians());
  for (auto &vertex : vertices) {
    const double relativeX = vertex.x - center.x;
    const double relativeY = vertex.y - center.y;

    const double rotatedX = (relativeX * cos_angle) - (relativeY * sin_angle);
    const double rotatedY = (relativeX * sin_angle) + (relativeY * cos_angle);
    vertex = Pos2d(rotatedX + center.x, rotatedY + center.y);
  }
}

// Returns vertices in counterclockwise direction starting at top right box
// corner
std::array<Pos2d, 4> GetBoxVertices(Pos2d box_size, Transform transform);

/*
    Indicates whether overlap occurred between the two projections of two edges
   and gives depth of overlap by modifying value of overlap parameter
*/
constexpr bool Overlap(Pos2d A, Pos2d B, double &overlap) {
  if (A.y >= B.x && B.y >= A.x) {
    const double overlap_start = std::max(A.x, B.x);
    const double overlap_end = std::min(A.y, B.y);
    overlap = overlap_end - overlap_start;
    return true;
  }
  overlap = -1.0;
  return false;
}

// Returns a collection of two normalized direction vectors representing the
// edges created by the given vertices.
std::array<Vec2d, 2> GetBoxAxes(const std::array<Pos2d, 4> &vertices);

// Returns normalized direction vector representing edge created by closest
// vertex and circle center
Vec2d GetCircleAxis(const isContainer auto &vertices, Pos2d circle_center) {
  double dist = math::INF;
  Vec2d smallestAxis;
  for (const auto &vertex : vertices) {
    const Vec2d edge = circle_center - vertex;
    const double d = math::Length(edge);
    if (d < dist) {
      dist = d;
      smallestAxis = edge;
    }
  }
  return math::Normalize(smallestAxis);
}
// Returns a projection of object, represented by given vertices, onto a
// specified axis
Vec2d Project(const isContainer auto &vertices, Vec2d axis) {
  double min = math::Dot(axis, vertices[0]);
  double max = min;

  for (auto it = std::next(vertices.begin()); it != vertices.end(); it++) {
    const double p = math::Dot(axis, *it);
    if (p < min) {
      min = p;
    } else if (p > max) {
      max = p;
    }
  }
  return Vector2(min, max);
}

// Returns a projection of circle, represented by given circle center and
// radius, onto a specified axis
Vec2d ProjectCircle(Pos2d circle_center, double radius, Vec2d axis);
} // namespace algo
} // namespace pe2d