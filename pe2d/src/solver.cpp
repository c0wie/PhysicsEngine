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
#include <iostream>

namespace pe2d {
void PositionSolver(std::vector<Collision> &collisions, float delta_time) {
  for (std::size_t i = 0; i < collisions.size(); i++) {
    RigidBody &rigid_bodyA = collisions[i].GetObjectA();
    RigidBody &rigid_bodyB = collisions[i].GetObjectB();

    const CollisionPoints &points = collisions[i].GetCollisionPoints();
    const Vector2 normal = points.Normal;

    Vector2 minimal_translation_vector = normal * points.Depth;
    if (math::Dot(minimal_translation_vector,
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

void ImpulseSolver(std::vector<Collision> &collisions, float delta_time) {
  for (auto &collision : collisions) {
    RigidBody &rigid_bodyA = collision.GetObjectA();
    RigidBody &rigid_bodyB = collision.GetObjectB();

    const double velocityA_magnitude = pe2d::math::Length(rigid_bodyA.GetLinearVelocity());
    const double velocityB_magnitude = pe2d::math::Length(rigid_bodyB.GetLinearVelocity());
    const double angleA = std::fmod(rigid_bodyA.GetAngle().AsDegrees(), 90.0);
    const double angleB = std::fmod(rigid_bodyB.GetAngle().AsDegrees(), 90.0);
    if(pe2d::math::NearlyEquel(velocityA_magnitude, 0.0, 1.5) &&
      pe2d::math::NearlyEquel(angleA, 0.0, 0.5) == true &&
      pe2d::math::NearlyEquel(velocityB_magnitude, 0.0, 1.5) && 
      pe2d::math::NearlyEquel(angleB, 0.0, 0.5) == true) {
        rigid_bodyA.sleep = true;
        rigid_bodyB.sleep = true;
        const double a = std::fmod(rigid_bodyA.GetAngle().AsDegrees(), 90.0);
        const double b = std::fmod(rigid_bodyA.GetAngle().AsDegrees(), 90.0);
        continue;
    }
    if(rigid_bodyA.sleep) {
      rigid_bodyA.sleep = false;
    }
    if(rigid_bodyB.sleep) {
      rigid_bodyB.sleep = false;
    }

    const CollisionPoints &points = collision.GetCollisionPoints();
    const double inv_massA = rigid_bodyA.GetInvMass();
    const double inv_massB = rigid_bodyB.GetInvMass();
    const double inv_inertiaA = rigid_bodyA.GetInvRotationalInertia();
    const double inv_inertiaB = rigid_bodyB.GetInvRotationalInertia();
    const Vector2 normal = points.Normal;
    const unsigned int contact_count = points.ContactCount;
    const double restitution_coefficient =
        (rigid_bodyA.GetRestitution() + rigid_bodyB.GetRestitution()) * 0.5;
    const std::array<Pos2d, 2> contact_list = {points.ContactPoint1,
                                               points.ContactPoint2};

    Vector2 minimal_translation_vector = normal * points.Depth;
    if (math::Dot(minimal_translation_vector,
                  rigid_bodyA.GetPosition() - rigid_bodyB.GetPosition()) <
        0.0f) {
      minimal_translation_vector *= -1.0f;
    }

    // list of vectors pointing from object A's center of mass to the contact
    // points
    std::array<Vec2d, 2> rA_list;
    // list of vectors pointing from object A's center of mass to the contact
    // points
    std::array<Vec2d, 2> rB_list;
    std::array<double, 2> impulse_scalars;
    // collision impulses along normal
    std::array<Vec2d, 2> impulses;
    // friction impulses along normal
    std::array<Vec2d, 2> friction_impulses;


    if (rigid_bodyA.IsStatic()) {
      rigid_bodyB.Move(-1.0 * minimal_translation_vector);
    } else if (rigid_bodyB.IsStatic()) {
      rigid_bodyA.Move(minimal_translation_vector);
    } else {
      rigid_bodyA.Move(minimal_translation_vector / 2.0);
      rigid_bodyB.Move(minimal_translation_vector / -2.0);
    }

    // calculate collision impulses
    for (std::size_t i = 0; i < contact_count; i++) {
      rA_list[i] = contact_list[i] - rigid_bodyA.GetPosition();
      rB_list[i] = contact_list[i] - rigid_bodyB.GetPosition();

      const Vector2 rA_perp = math::Perp(rA_list[i]);
      const Vector2 rB_perp = math::Perp(rB_list[i]);

      const Vector2 relative_velocity =
          (rigid_bodyA.GetLinearVelocity() +
           (rA_perp * rigid_bodyA.GetAngularVelocity())) -
          rigid_bodyB.GetLinearVelocity() -
          (rB_perp * rigid_bodyB.GetAngularVelocity());

      const double rA_perp_normal = math::Dot(rA_perp, normal);
      const double rB_perp_normal = math::Dot(rB_perp, normal);

      const double denominator =
          inv_massA + inv_massB +
          (rA_perp_normal * rA_perp_normal) * inv_inertiaA +
          (rB_perp_normal * rB_perp_normal) * inv_inertiaB;

      double impulse_scalar = -(1.0 + restitution_coefficient) *
                              math::Dot(relative_velocity, normal);
      impulse_scalar /= denominator;
      impulse_scalar /= (double)contact_count;
      impulse_scalars[i] = impulse_scalar;

      impulses[i] = impulse_scalar * normal;
      // std::cout << impulses[i].GetString() << std::endl;
    }

    // apply collision impulses
    for (std::size_t i = 0; i < contact_count; i++) {
      const Vector2 impulse = impulses[i];
      rigid_bodyA.AddLinearVelocity(impulse * inv_massA);
      rigid_bodyA.AddAngularVelocity(math::Cross(rA_list[i], impulse) *
                                     inv_inertiaA);
      rigid_bodyB.AddLinearVelocity(impulse * -inv_massB);
      rigid_bodyB.AddAngularVelocity(math::Cross(rB_list[i], impulse) *
                                     -inv_inertiaB);
    }

    const double static_friction_coefficient =
        (rigid_bodyA.GetStaticFriction() + rigid_bodyB.GetStaticFriction()) *
        0.5;

    const double dynamic_friction_coefficient =
        (rigid_bodyA.GetDynamicFriction() + rigid_bodyB.GetDynamicFriction()) *
        0.5;

    // calculate friction impulses
    for (std::size_t i = 0; i < contact_count; i++) {
      rA_list[i] = contact_list[i] - rigid_bodyA.GetPosition();
      rB_list[i] = contact_list[i] - rigid_bodyB.GetPosition();

      const Vector2 rA_perp = math::Perp(rA_list[i]);
      const Vector2 rB_perp = math::Perp(rB_list[i]);

      const Vector2 relative_velocity =
          (rigid_bodyA.GetLinearVelocity() +
           (rA_perp * rigid_bodyA.GetAngularVelocity())) -
          (rigid_bodyB.GetLinearVelocity() +
           (rB_perp * rigid_bodyB.GetAngularVelocity()));

      Vector2 tangent =
          relative_velocity - math::Dot(relative_velocity, normal) * normal;
          tangent = pe2d::math::Normalize(tangent);

      const float rA_perp_tangent = math::Dot(rA_perp, tangent);
      const float rB_perp_tangent = math::Dot(rB_perp, tangent);

      const double denominator =
          inv_massA + inv_massB +
          (rA_perp_tangent * rA_perp_tangent) * inv_inertiaA +
          (rB_perp_tangent * rB_perp_tangent) * inv_inertiaB;

      float friction_impulse_scalar =
          math::Dot(relative_velocity, tangent) /* * -1.0*/;
      friction_impulse_scalar /= denominator;
      friction_impulse_scalar /= (double)contact_count;

      const float impulse_scalar = impulse_scalars[i];
      if (std::abs(friction_impulse_scalar) <=
          impulse_scalar * static_friction_coefficient) {
        friction_impulses[i] = friction_impulse_scalar * tangent;
      } else {
        friction_impulses[i] =
            impulse_scalar * tangent * dynamic_friction_coefficient;
      }
      if (std::abs(friction_impulses[i].y) < 35.0) {
        friction_impulses[i] = Vec2d(friction_impulses[i].x, 0.0);
      }
    }

    // apply friction impulses
    for (std::size_t i = 0; i < contact_count; i++) {
      Vector2 friction_impulse = friction_impulses[i];
      rigid_bodyA.AddLinearVelocity(friction_impulse * inv_massA);
      rigid_bodyA.AddAngularVelocity(math::Cross(rA_list[i], friction_impulse) *
                                     inv_inertiaA);
      rigid_bodyB.AddLinearVelocity(friction_impulse * -inv_massB);
      rigid_bodyB.AddAngularVelocity(math::Cross(rB_list[i], friction_impulse) *
                                     -inv_inertiaB);
    }
  }
}

} // namespace pe2d