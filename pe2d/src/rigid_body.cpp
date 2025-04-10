// header
#include "rigid_body.hpp"

namespace pe2d {

RigidBody::RigidBody(BodyType type, Vec2f size, Transform transform,
                     float mass, bool is_static, Vec2f gravity)
    : CollisionBody(type, size, transform.position, gravity, is_static) {
  SetScale(transform.scale);
  SetAngle(transform.angle);
  if (m_IsStatic) {
    m_Mass = math::INF;
    m_RotationalInertia = math::INF;
  } else {
    SetMass(mass);
  }
}

RigidBody::RigidBody(BodyType type, Vec2f size, Transform transform,
                     float mass, bool is_static, Vec2f gravity,
                     Vec2f linear_velocity, float angular_velocity)
    : CollisionBody(type, size, transform.position, gravity, linear_velocity, is_static), m_Mass(mass),
      m_AngularVelocity(angular_velocity) {
  SetScale(transform.scale);
  SetAngle(transform.angle);
  if (m_IsStatic) {
    m_Mass = math::INF;
    m_RotationalInertia = math::INF;
  } else {
    SetMass(mass);
  }
}

RigidBody::RigidBody(BodyType type, Vec2f size, Transform transform,
                     float mass, bool is_static, Vec2f gravity,
                     Vec2f linear_velocity, float angular_velocity,
                     float static_friction, float dynamic_friction,
                     float restitution)
    : CollisionBody(type, size, transform.position, gravity, linear_velocity, is_static), m_AngularVelocity(angular_velocity) {
  SetScale(transform.scale);
  SetAngle(transform.angle);
  if (m_IsStatic) {
    m_Mass = math::INF;
    m_RotationalInertia = math::INF;
  } else {
    SetMass(mass);
  }
  SetStaticFriction(static_friction);
  SetDynamicFriction(dynamic_friction);
  SetRestitution(restitution);
}
} // namespace pe2d
