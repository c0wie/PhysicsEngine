// header
#include "collider.hpp"

// local
#include "algo.hpp"
#include "rigid_body.hpp"
#include "vector2.hpp"

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
  return {};
}

CollisionPoints FindCircleCircleCollision(float radiusA,
                                          Transform circle_transformA,
                                          float radiusB,
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
  const Vec2f centerA = circle_transformA.position;
  const Vec2f centerB = cricle_transformB.position;
  Vec2f diff = centerA - centerB;

  const float &length = math::Length(diff);
  // with circle I scaled them based on x value of scale
  const float sum = (radiusA * circle_transformA.scale.x) +
                     (radiusB * cricle_transformB.scale.x);
  if (length > sum) {
    return {};
  }
  const float overlap = sum - length;
  Vec2f normal = math::Normalize(diff);
  const Vec2f contact_point = centerA - normal * radiusA;
  return CollisionPoints(normal, overlap, contact_point);
}

CollisionPoints FindCircleBoxCollision(float radius,
                                       Transform circle_transform,
                                       Vec2f box_size,
                                       Transform box_transform) {
  if (radius <= 0.0f) {
    throw std::invalid_argument("[FindCircleBoxCollision()] Error: radius "
                                "must be positive (received " +
                                std::to_string(radius) + ")");
  }
  if (box_size.x <= 0.0f || box_size.y <= 0.0f) {
    throw std::invalid_argument("[FindCircleBoxCollision()] Error: box_size "
                                "must be positive (received " +
                                box_size.GetString() + ")");
  }
  Vec2f circle_center = circle_transform.position;
  const std::array<Vec2f, 4> box_vertices =
      algo::GetBoxVertices(box_size, box_transform);
  const std::array<Vec2f, 2> box_axes = algo::GetBoxAxes(box_vertices);
  const std::array<Vec2f, 3> all_axes = {
      box_axes[0], box_axes[1],
      algo::GetCircleAxis(box_vertices, circle_center)};
  Vec2f smallest_axis;
  float overlap = math::INF;

  for (const auto &axis : all_axes) {
    const Vec2f circle_projection = algo::ProjectCircle(
        circle_center, radius * circle_transform.scale.x, axis);
    const Vec2f box_projection = algo::Project(box_vertices, axis);
    float o = 0.0;
    if (!algo::Overlap(circle_projection, box_projection, o)) {
      return {};
    }
    if (o < overlap) {
      overlap = o;
      smallest_axis = axis;
    }
  }

  const Vec2f contact_point =
      algo::FindCircleBoxContactPoint(box_vertices, circle_center);

  return CollisionPoints(-1.0f * smallest_axis, overlap, contact_point);
}
CollisionPoints FindBoxCircleCollision(Vec2f box_size, Transform box_transform,
                                       float radius,
                                       Transform circle_transform) {
  if (box_size.x <= 0.0f || box_size.y <= 0.0f) {
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
  p.normal *= -1.0f;
  return p;
}
CollisionPoints FindBoxBoxCollision(Vec2f box_sizeA, Transform box_transformA,
                                    Vec2f box_sizeB,
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
  const std::array<Vec2f, 4> verticesA =
      algo::GetBoxVertices(box_sizeA, box_transformA);
  const std::array<Vec2f, 4> verticesB =
      algo::GetBoxVertices(box_sizeB, box_transformB);
  const std::array<Vec2f, 2> axesA = algo::GetBoxAxes(verticesA);
  const std::array<Vec2f, 2> axesB = algo::GetBoxAxes(verticesB);
  Vec2f smallest_axis;
  float overlap = math::INF;

  for (int i = 0; i < axesA.size(); i++) {
    const Vec2f pA1 = algo::Project(verticesA, axesA[i]);
    const Vec2f pA2 = algo::Project(verticesB, axesA[i]);
    float overlapA = 0.0;
    if (!algo::Overlap(pA1, pA2, overlapA)) {
      return CollisionPoints();
    }
    const Vec2f pB1 = algo::Project(verticesA, axesB[i]);
    const Vec2f pB2 = algo::Project(verticesB, axesB[i]);
    float overlapB = 0.0f;
    if (!algo::Overlap(pB1, pB2, overlapB)) {
      return CollisionPoints();
    }
    if (overlapA < overlap) {
      overlap = overlapA;
      smallest_axis = axesA[i];
    }
    if (overlapB < overlap) {
      overlap = overlapB;
      smallest_axis = axesB[i];
    }
  }
  const std::pair<Vec2f, Vec2f> contact_points =
      algo::FindBoxBoxContactPoint(verticesA, verticesB);
  if (contact_points.second == Vec2f(-1.0, -1.0)) {
    return CollisionPoints(smallest_axis, overlap, contact_points.first);
  }
  return CollisionPoints(smallest_axis, overlap, contact_points.first,
                         contact_points.second);
}
} // namespace pe2d