// header
#include "collider.hpp"

// local
#include "algo.hpp"
#include "rigid_body.hpp"
#include "vector2.hpp"
#include <stdexcept>

namespace pe2d {
CollisionPoints TestCollison(const RigidBody &a, const RigidBody &b) {
  if (a.GetType() == Circle && b.GetType() == Circle) {
    return FindCircleCircleCollision(a.GetSize().x, a.GetTransform(),
                                     b.GetSize().x, b.GetTransform());
  } else if (a.GetType() == Circle && b.GetType() == Box) {
    return FindCircleBoxCollision(a.GetSize().x, a.GetTransform(), b.GetSize(),
                                  b.GetTransform());
  } else if (a.GetType() == Box && b.GetType() == Circle) {
    return FindBoxCircleCollision(a.GetSize(), a.GetTransform(), b.GetSize().x,
                                  b.GetTransform());
  } else if (a.GetType() == Box && b.GetType() == Box) {
    return FindBoxBoxCollision(a.GetSize(), a.GetTransform(), b.GetSize(),
                               b.GetTransform());
  }
  throw std::invalid_argument("");
  return {};
}

CollisionPoints FindCircleCircleCollision(double radiusA,
                                          Transform circle_transformA,
                                          double radiusB,
                                          Transform cricle_transformB) {
  if (radiusA <= 0.0) {
    throw std::invalid_argument("[FindCircleCircleCollision()] Error: radiusA "
                                "must be positive (received " +
                                std::to_string(radiusA) + ")");
  }
  if (radiusB <= 0.0) {
    throw std::invalid_argument("[FindCircleCircleCollision()] Error: radiusB "
                                "must be positive (received " +
                                std::to_string(radiusB) + ")");
  }
  const Vector2 centerA = circle_transformA.position;
  const Vector2 centerB = cricle_transformB.position;
  const Vector2 diff = centerA - centerB;
  const double &length = math::Length(diff);
  // with circle I scaled them based on x value of scale
  double sum = (radiusA * circle_transformA.scale.x) +
               (radiusB * cricle_transformB.scale.x);
  if (length > sum) {
    return {};
  }
  const double overlap = sum - length;
  Vector2 normal = math::Normalize(diff);
  const Vector2 contact_point = centerA - normal * radiusA;
  return CollisionPoints(normal, overlap, contact_point, true);
}

CollisionPoints FindCircleBoxCollision(double radius,
                                       Transform circle_transform,
                                       Size2d box_size,
                                       Transform box_transform) {
  if (radius <= 0.0) {
    throw std::invalid_argument("[FindCircleBoxCollision()] Error: radius "
                                "must be positive (received " +
                                std::to_string(radius) + ")");
  }
  if (box_size.x <= 0.0 || box_size.y <= 0.0) {
    throw std::invalid_argument("[FindCircleBoxCollision()] Error: box_size "
                                "must be positive (received " +
                                box_size.GetString() + ")");
  }
  Vector2 circleCenter = circle_transform.position;
  const std::array<Pos2d, 4> box_vertices =
      algo::GetBoxVertices(box_size, box_transform);
  const std::array<Vec2d, 2> box_axes = algo::GetBoxAxes(box_vertices);
  const std::array<Vec2d, 3> all_axes = {
      box_axes[0], box_axes[1],
      algo::GetCircleAxis(box_vertices, circleCenter)};
  const Vec2d *smallesAxis = nullptr;
  double overlap = math::INF;
  for (std::size_t i = 0; i < all_axes.size(); i++) {
    const Vec2d p1 = algo::ProjectCircle(
        circleCenter, radius * circle_transform.scale.x, all_axes[i]);
    const Vec2d p2 = algo::Project(box_vertices, all_axes[i]);
    double o = 0.0;
    if (!algo::Overlap(p1, p2, o)) {
      return {};
    }
    if (o < overlap) {
      overlap = o;
      smallesAxis = &all_axes[i];
    }
  }
  const Vector2 contact_point =
      algo::FindCircleBoxContactPoint(box_vertices, circleCenter);
  return CollisionPoints(*smallesAxis, overlap, contact_point, true);
}
CollisionPoints FindBoxCircleCollision(Size2d box_size, Transform box_transform,
                                       double radius,
                                       Transform circle_transform) {
  if (box_size.x <= 0.0 || box_size.y <= 0.0) {
    throw std::invalid_argument("[FindBoxCircleCollision()] Error: box_size "
                                "must be positive (received: " +
                                box_size.GetString() + ")");
  }
  if (radius <= 0.0f) {
    throw std::invalid_argument("[FindBoxCircleCollision()] Error: radius must "
                                "be positive (received: " +
                                std::to_string(radius) + ")");
  }
  CollisionPoints p =
      FindCircleBoxCollision(radius, circle_transform, box_size, box_transform);
  p.Normal *= -1.0f;
  return p;
}
CollisionPoints FindBoxBoxCollision(Size2d box_sizeA, Transform box_transformA,
                                    Size2d box_sizeB,
                                    Transform box_transformB) {
  if (box_sizeA.x <= 0.0 || box_sizeA.y <= 0.0) {
    throw std::invalid_argument("[FindBoxBoxCollision()] Error: box_sizeA "
                                "must be positive (received " +
                                box_sizeA.GetString() + ")");
  }
  if (box_sizeB.x <= 0.0 || box_sizeB.y <= 0.0) {
    throw std::invalid_argument("[FindBoxBoxCollision()] Error: box_sizeB "
                                "must be positive (received " +
                                box_sizeB.GetString() + ")");
  }
  const std::array<Pos2d, 4> verticesA =
      algo::GetBoxVertices(box_sizeA, box_transformA);
  const std::array<Pos2d, 4> verticesB =
      algo::GetBoxVertices(box_sizeB, box_transformB);
  const std::array<Vec2d, 2> axesA = algo::GetBoxAxes(verticesA);
  const std::array<Vec2d, 2> axesB = algo::GetBoxAxes(verticesB);
  const Vec2d *smallest_axis = nullptr;
  double overlap = math::INF;
  for (int i = 0; i < axesA.size(); i++) {
    const Vector2 pA1 = algo::Project(verticesA, axesA[i]);
    const Vector2 pA2 = algo::Project(verticesB, axesA[i]);
    double overlapA = 0.0f;
    if (!algo::Overlap(pA1, pA2, overlapA)) {
      return CollisionPoints();
    }
    const Vector2 pB1 = algo::Project(verticesA, axesB[i]);
    const Vector2 pB2 = algo::Project(verticesB, axesB[i]);
    double overlapB = 0.0f;
    if (!algo::Overlap(pB1, pB2, overlapB)) {
      return CollisionPoints();
    }
    if (overlapA < overlap) {
      overlap = overlapA;
      smallest_axis = &axesA[i];
    }
    if (overlapB < overlap) {
      overlap = overlapB;
      smallest_axis = &axesB[i];
    }
  }
  std::pair<Pos2d, Pos2d> contactPoints =
      algo::FindBoxBoxContactPoint(verticesA, verticesB);
  if (contactPoints.second == Pos2d(-1.0f, -1.0f)) {
    return CollisionPoints(*smallest_axis, overlap, contactPoints.first, true);
  }
  return CollisionPoints(*smallest_axis, overlap, contactPoints.first,
                         contactPoints.second, true);
}
} // namespace pe2d