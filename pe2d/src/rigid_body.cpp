// header
#include "rigid_body.hpp"

// local
#include "algo.hpp"
#include "angle.hpp"
#include "assert.hpp"
#include "math.hpp"
#include "vector2.hpp"
#include <cmath>

namespace pe2d {

RigidBody::RigidBody(std::size_t id, RigidBodyType type, Vec2f size,
                     Transform transform, float mass, bool is_static,
                     Vec2f gravity)
    : m_ID(id), m_Type(type), m_Transform(transform), m_IsStatic(is_static),
      m_Gravity(gravity) {
  SetSize(size);
  if (m_IsStatic) {
    m_Mass = math::INF;
    m_RotationalInertia = math::INF;
  } else {
    SetMass(mass);
  }
}

RigidBody::RigidBody(std::size_t id, RigidBodyType type, Vec2f size,
                     Transform transform, float mass, bool is_static,
                     Vec2f gravity, Vec2f linear_velocity,
                     float angular_velocity)
    : m_ID(id), m_Type(type), m_Transform(transform), m_Mass(mass),
      m_IsStatic(is_static), m_Gravity(gravity),
      m_LinearVelocity(linear_velocity), m_AngularVelocity(angular_velocity) {
  SetSize(size);
  if (m_IsStatic) {
    m_Mass = math::INF;
    m_RotationalInertia = math::INF;
  } else {
    SetMass(mass);
  }
}

RigidBody::RigidBody(std::size_t id, RigidBodyType type, Vec2f size,
                     Transform transform, float mass, bool is_static,
                     Vec2f gravity, Vec2f linear_velocity,
                     float angular_velocity, float static_friction,
                     float dynamic_friction, float restitution)
    : m_ID(id), m_Type(type), m_Transform(transform), m_IsStatic(is_static),
      m_Gravity(gravity), m_LinearVelocity(linear_velocity),
      m_AngularVelocity(angular_velocity) {
  SetSize(size);
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

std::array<Vec2f, 4> RigidBody::GetBoundingBox() const {
  ASSERT(m_Type == Box || m_Type == Circle, "m_Type isn't defined");
  ASSERT(m_Size.x > 0.0 || m_Size.y > 0.0, "m_Size isn't initialized");
  if (m_Type == Circle) {
    const float diameter = m_Size.x * 2;
    return algo::GetBoxVertices(
        Vec2f(diameter, diameter),
        Transform{m_Transform.position, Angle(), m_Transform.scale});
  }
  const std::array<Vec2f, 4> vertices =
      algo::GetBoxVertices(m_Size, m_Transform);
  Vec2f top_left_corner = Vec2f(math::INF, math::INF);
  Vec2f bot_right_corner = Vec2f(-math::INF, -math::INF);
  for (const auto vertex : vertices) {
    top_left_corner.x = std::min(top_left_corner.x, vertex.x);
    top_left_corner.y = std::min(top_left_corner.y, vertex.y);
    bot_right_corner.x = std::max(bot_right_corner.x, vertex.x);
    bot_right_corner.y = std::max(bot_right_corner.y, vertex.y);
  }
  const Vec2f top_right_corner = Vec2f(bot_right_corner.x, top_left_corner.y);
  const Vec2f bot_left_corner = Vec2f(top_left_corner.x, bot_right_corner.y);

  return {top_right_corner, top_left_corner, bot_left_corner, bot_right_corner};
}
} // namespace pe2d
