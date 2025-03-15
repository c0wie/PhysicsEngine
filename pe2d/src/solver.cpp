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
    // collision impulses along normal
    std::array<Vec2d, 2> impulses;
    // friction impulses along normal
    std::array<Vec2d, 2> friction_impulses;

    std::array<double, 2> j_list;

    if (rigid_bodyA.IsStatic()) {
      rigid_bodyB.Move(-1.0f * minimal_translation_vector);
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
          (rigid_bodyB.GetLinearVelocity() +
           (rB_perp * rigid_bodyB.GetAngularVelocity()));

      const double rA_perp_normal = math::Dot(rA_perp, normal);
      const double rB_perp_normal = math::Dot(rB_perp, normal);

      const double denominator =
          inv_massA + inv_massB +
          (rA_perp_normal * rA_perp_normal) * inv_inertiaA +
          (rB_perp_normal * rB_perp_normal) * inv_inertiaB;

      double j = -(1.0 + restitution_coefficient) *
                 math::Dot(relative_velocity, normal);
      j /= denominator;
      j /= (double)contact_count;
      j_list[i] = j;

      impulses[i] = j * normal;
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

      Vector2 tangent  = relative_velocity * math::Dot(relative_velocity, normal) * normal;
      if(math::NearlyEquel(tangent, pe2d::Vec2d(), 0.00000025)) {
        continue;
      } else {
        tangent = pe2d::math::Normalize(tangent);
        std::cout << "Relative velocity: " << relative_velocity.GetString() << '\n';
      }

      const float rA_perp_tangent = math::Dot(rA_perp, tangent);
      const float rB_perp_tangent = math::Dot(rB_perp, tangent);

      const double denominator =
          inv_massA + inv_massB +
          (rA_perp_tangent * rA_perp_tangent) * inv_inertiaA +
          (rB_perp_tangent * rB_perp_tangent) * inv_inertiaB;

      float jt = -math::Dot(relative_velocity, tangent);
      jt /= denominator;
      jt /= (double)contact_count;

      const float j = j_list[i];
      if (std::abs(jt) <= j * static_friction_coefficient) {
        friction_impulses[i] = jt * tangent;
      } else {
        friction_impulses[i] = j * tangent * dynamic_friction_coefficient;
      }
    }
    for (int i = 0; i < contact_count; i++) {
      Vector2 friction_impulse = friction_impulses[i];
      if(math::Length(friction_impulse) > math::Length(impulses[i])) {
        // Normalize the friction impulse to get its direction
        const Vector2 friction_direction = math::Normalize(friction_impulse);

        // Scale the direction by the magnitude of the collision impulse
        friction_impulse = friction_direction * math::Length(impulses[i]);
      }
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