// header
#include "solver.hpp"

// local
#include "collision_points.hpp"
#include "math.hpp"
#include "rigid_body.hpp"
#include "vector2.hpp"

// std
#include <array>
#include <cstddef>

namespace pe2d {
void PositionSolver(std::vector<Collision> &collisions, float delta_time) {
  for (std::size_t i = 0; i < collisions.size(); i++) {
    RigidBody &rigid_bodyA = collisions[i].GetObjectA();
    RigidBody &rigid_bodyB = collisions[i].GetObjectB();

    const CollisionPoints &points = collisions[i].GetCollisionPoints();
    const Vector2 normal = points.Normal;

    Vector2 minimal_translation_vector = normal * points.Depth;
    if (pe2d::math::Dot(minimal_translation_vector,
                        rigid_bodyA.GetPosition() - rigid_bodyB.GetPosition()) <
        0.0) {
      minimal_translation_vector *= -1.0;
    }

    if (rigid_bodyA.IsStatic()) {
      rigid_bodyB.Move(-1.0 * minimal_translation_vector);
    } else if (rigid_bodyB.IsStatic()) {
      rigid_bodyA.Move(minimal_translation_vector);
    } else {
      rigid_bodyA.Move(minimal_translation_vector / 2.0);
      rigid_bodyB.Move(minimal_translation_vector / -2.0);
    }
  }
}

void ImpulseSolverWithoutFriction(std::vector<Collision> &collisions,
                                  float delta_time) {
  for (std::size_t i = 0; i < collisions.size(); i++) {
    RigidBody &rigid_bodyA = collisions[i].GetObjectA();
    RigidBody &rigid_bodyB = collisions[i].GetObjectB();
    
    const CollisionPoints &points = collisions[i].GetCollisionPoints();
    const double inv_massA = rigid_bodyA.GetInvMass();
    const double inv_massB = rigid_bodyB.GetInvMass();
    const double inv_inertiaA = rigid_bodyA.GetInvRotationalInertia();
    const double inv_inertiaB = rigid_bodyB.GetInvRotationalInertia();
    const Vector2 normal = points.Normal;
    const unsigned int contact_count = points.ContactCount;
    const double restitution_coefficient =
        (rigid_bodyA.GetRestitution() + rigid_bodyB.GetRestitution()) * 0.5f;
    const std::array<Pos2d, 2> contact_list = {points.ContactPoint1,
                                               points.ContactPoint2};
    // list of vectors pointing from object A's center of mass to the contact
    // points
    std::array<Vec2d, 2> rA_list;
    // list of vectors pointing from object A's center of mass to the contact
    // points
    std::array<Vec2d, 2> rB_list;
    // list of collision impulses
    std::array<Vec2d, 2> impulse_list;

    Vector2 minimal_translation_vector = normal * points.Depth;
    if (pe2d::math::Dot(minimal_translation_vector,
                        rigid_bodyA.GetPosition() - rigid_bodyB.GetPosition()) <
        0.0f) {
      minimal_translation_vector *= -1.0f;
    }
    if (rigid_bodyA.IsStatic()) {
      rigid_bodyB.Move(-1.0f * minimal_translation_vector);
    } else if (rigid_bodyB.IsStatic()) {
      rigid_bodyA.Move(minimal_translation_vector);
    } else {
      rigid_bodyA.Move(minimal_translation_vector / 2.0f);
      rigid_bodyB.Move(minimal_translation_vector / -2.0f);
    }

    for (std::size_t i = 0; i < contact_count; i++) {
      rA_list[i] = contact_list[i] - rigid_bodyA.GetPosition();
      rB_list[i] = contact_list[i] - rigid_bodyB.GetPosition();

      const Vector2 rA_perp = pe2d::math::Perp(rA_list[i]);
      const Vector2 rB_perp = pe2d::math::Perp(rB_list[i]);

      const Vector2 relative_velocity =
          (rigid_bodyA.GetLinearVelocity() +
           (rA_perp * rigid_bodyA.GetAngularVelocity())) -
          (rigid_bodyB.GetLinearVelocity() +
           (rB_perp * rigid_bodyB.GetAngularVelocity()));

      const double relative_velocity_along_normal =
          pe2d::math::Dot(relative_velocity, normal);

      const double rA_perp_normal = pe2d::math::Dot(rA_perp, normal);
      const double rB_perp_normal = pe2d::math::Dot(rB_perp, normal);

      const double denominator =
          inv_massA + inv_massB +
          (rA_perp_normal * rA_perp_normal) * inv_inertiaA +
          (rB_perp_normal * rB_perp_normal) * inv_inertiaB;

      double impulse =
          -(1.0f + restitution_coefficient) * relative_velocity_along_normal;
      impulse /= denominator;
      impulse /= (double)contact_count;

      impulse_list[i] = impulse * normal;
    }

    for (std::size_t i = 0; i < contact_count; i++) {
      const Vector2 impulse = impulse_list[i];
      rigid_bodyA.AddLinearVelocity(impulse * inv_massA);
      rigid_bodyA.AddAngularVelocity(pe2d::math::Cross(rA_list[i], impulse) *
                                     inv_inertiaA);
      rigid_bodyB.AddLinearVelocity(impulse * -inv_massB);
      rigid_bodyB.AddAngularVelocity(pe2d::math::Cross(rB_list[i], impulse) *
                                     -inv_inertiaB);
    }
  }
}

void ImpulseSolverWithFriction(std::vector<Collision> &collisions,
                               float delta_time) {
  for (auto &collision : collisions) {
    RigidBody &rigid_bodyA = collision.GetObjectA();
    RigidBody &rigid_bodyB = collision.GetObjectB();
    const CollisionPoints &points = collision.GetCollisionPoints();
    const double inv_massA = rigid_bodyA.GetInvMass();
    const double inv_massB = rigid_bodyB.GetInvMass();
    const double inv_inertiaA = rigid_bodyA.GetInvRotationalInertia();
    const double inv_inertiaB = rigid_bodyB.GetInvRotationalInertia();
    const Vector2 normal = points.Normal;
    const unsigned int contact_count = points.ContactCount;
    const double restitution_coefficient =
        (rigid_bodyA.GetRestitution() + rigid_bodyB.GetRestitution()) * 0.5f;
    const double static_friction_coefficient =
        (rigid_bodyA.GetStaticFriction() + rigid_bodyB.GetStaticFriction()) *
        0.5f;
    const double dynamic_friction_coefficient =
        (rigid_bodyA.GetDynamicFriction() + rigid_bodyB.GetDynamicFriction()) *
        0.5f;
    const std::array<Pos2d, 2> contact_list = {points.ContactPoint1,
                                               points.ContactPoint2};
    // list of vectors pointing from object A's center of mass to the contact
    // points
    std::array<Vec2d, 2> rA_list;
    // list of vectors pointing from object A's center of mass to the contact
    // points
    std::array<Vec2d, 2> rB_list;
    // list of collision impulses
    std::array<Vec2d, 2> impulse_list;
    std::array<Vec2d, 2> friction_impulse_list;
    std::array<double, 2> j_list;
    Vector2 minimal_translation_vector = normal * points.Depth;

    if (pe2d::math::Dot(minimal_translation_vector,
                        rigid_bodyA.GetPosition() - rigid_bodyB.GetPosition()) <
        0.0) {
      minimal_translation_vector *= -1.0;
    }
    if (rigid_bodyA.IsStatic()) {
      rigid_bodyB.Move(-1.0 * minimal_translation_vector);
    } else if (rigid_bodyB.IsStatic()) {
      rigid_bodyA.Move(minimal_translation_vector);
    } else {
      rigid_bodyA.Move(minimal_translation_vector / 2.0);
      rigid_bodyB.Move(minimal_translation_vector / -2.0);
    }

    for (int i = 0; i < contact_count; i++) {
      // vector pointing from center of mass of the objects to the contact
      // points
      rA_list[i] = contact_list[i] - rigid_bodyA.GetPosition();
      rB_list[i] = contact_list[i] - rigid_bodyB.GetPosition();

      const Vector2 rA_perp = pe2d::math::Perp(rA_list[i]);
      const Vector2 rB_perp = pe2d::math::Perp(rB_list[i]);

      const Vector2 relative_velocity =
          (rigid_bodyA.GetLinearVelocity() +
           (rA_perp * rigid_bodyA.GetAngularVelocity())) -
          (rigid_bodyB.GetLinearVelocity() +
           (rB_perp * rigid_bodyB.GetAngularVelocity()));

      const float relative_velocity_along_normal =
          pe2d::math::Dot(relative_velocity, normal);

      if (relative_velocity_along_normal > 0.0f) {
        continue;
      }

      const float rA_perp_normal = pe2d::math::Dot(rA_perp, normal);
      const float rB_perp_normal = pe2d::math::Dot(rB_perp, normal);

      const float denominator =
          inv_massA + inv_massB +
          (rA_perp_normal * rA_perp_normal) * inv_inertiaA +
          (rB_perp_normal * rB_perp_normal) * inv_inertiaB;

      double j =
          -(1.0 + restitution_coefficient) * relative_velocity_along_normal;
      j /= denominator;
      j /= (float)contact_count;
      j_list[i] = j;

      impulse_list[i] = j * normal;
    }
    for (int i = 0; i < contact_count; i++) {
      const Vector2 impulse = impulse_list[i];
      rigid_bodyA.AddLinearVelocity(impulse * inv_massA);
      rigid_bodyA.AddAngularVelocity(pe2d::math::Cross(rA_list[i], impulse) *
                                     inv_inertiaA);
      rigid_bodyB.AddLinearVelocity(impulse * -inv_massB);
      rigid_bodyB.AddAngularVelocity(pe2d::math::Cross(rB_list[i], impulse) *
                                     -inv_inertiaB);
    }

    for (int i = 0; i < contact_count; i++) {
      const Vector2 rA_perp = pe2d::math::Perp(rA_list[i]);
      const Vector2 rB_perp = pe2d::math::Perp(rB_list[i]);

      const auto angVelA = rA_perp * rigid_bodyA.GetAngularVelocity();
      const auto angVelB = rB_perp * rigid_bodyB.GetAngularVelocity();

      const Vector2 relative_velocity =
          (rigid_bodyA.GetLinearVelocity() + angVelA) -
          (rigid_bodyB.GetLinearVelocity() + angVelB);

      Vector2 tangent = relative_velocity -
                        pe2d::math::Dot(relative_velocity, normal) * normal;

      if (pe2d::math::NearlyEquel(tangent, {0.0f, 0.0f}, 0.0005f)) {
        continue;
      } else {
        tangent = pe2d::math::Normalize(tangent);
      }

      const float rA_perpTang = pe2d::math::Dot(rA_perp, tangent);
      const float rB_perpTang = pe2d::math::Dot(rB_perp, tangent);

      const float frictionDenominator =
          inv_massA + inv_massB + (rA_perpTang * rA_perpTang) * inv_inertiaA +
          (rB_perpTang * rB_perpTang) * inv_inertiaB;

      float jt = -pe2d::math::Dot(relative_velocity, tangent);
      jt /= frictionDenominator;
      jt /= (float)contact_count;

      const float j = j_list[i];
      if (std::abs(jt) <= j * static_friction_coefficient) {
        friction_impulse_list[i] = jt * tangent;
      } else {
        friction_impulse_list[i] = -j * tangent * dynamic_friction_coefficient;
      }
    }

    for (int i = 0; i < contact_count; i++) {
      const Vector2 friction_impulse = friction_impulse_list[i];

      rigid_bodyA.AddLinearVelocity(friction_impulse * inv_massA);
      rigid_bodyA.AddAngularVelocity(
          pe2d::math::Cross(rA_list[i], friction_impulse) * inv_inertiaA);
      rigid_bodyB.AddLinearVelocity(friction_impulse * -inv_massB);
      rigid_bodyB.AddAngularVelocity(
          pe2d::math::Cross(rB_list[i], friction_impulse) * -inv_inertiaB);
    }
  }
}
} // namespace pe2d