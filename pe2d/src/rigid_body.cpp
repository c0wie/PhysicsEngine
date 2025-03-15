// header
#include "rigid_body.hpp"

// local
#include "algo.hpp"
#include "angle.hpp"
#include "assert.hpp"
#include "math.hpp"
#include "vector2.hpp"

namespace pe2d {

RigidBody::RigidBody(std::size_t id, RigidBodyType type, Size2d size,
                     Transform transform, double mass, bool is_static,
                     Vec2d gravity)
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

RigidBody::RigidBody(std::size_t id, RigidBodyType type, Size2d size,
                     Transform transform, double mass, bool is_static,
                     Vec2d gravity, Vec2d linear_velocity,
                     double angular_velocity)
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

RigidBody::RigidBody(std::size_t id, RigidBodyType type, Size2d size,
                     Transform transform, double mass, bool is_static,
                     Vec2d gravity, Vec2d linear_velocity,
                     double angular_velocity, double static_friction,
                     double dynamic_friction, double restitution)
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

std::array<Pos2d, 4> RigidBody::GetBoundingBox() const {
  ASSERT(m_Type == Box || m_Type == Circle, "m_Type isn't defined");
  ASSERT(m_Size.x > 0.0 || m_Size.y > 0.0, "m_Size isn't initialized");
  if (m_Type == Circle) {
    const double diameter = m_Size.x * 2;
    return algo::GetBoxVertices(
        Size2d(diameter, diameter),
        Transform{m_Transform.position, Angle(), m_Transform.scale});
  }
  const std::array<Pos2d, 4> vertices =
      algo::GetBoxVertices(m_Size, m_Transform);
  Pos2d top_left_corner = Pos2d(math::INF, math::INF);
  Pos2d bot_right_corner = Pos2d(-math::INF, -math::INF);
  for (const auto vertex : vertices) {
    top_left_corner.x = std::min(top_left_corner.x, vertex.x);
    top_left_corner.y = std::min(top_left_corner.y, vertex.y);
    bot_right_corner.x = std::max(bot_right_corner.x, vertex.x);
    bot_right_corner.y = std::max(bot_right_corner.y, vertex.y);
  }
  const Pos2d top_right_corner = Pos2d(bot_right_corner.x, top_left_corner.y);
  const Pos2d bot_left_corner = Pos2d(top_left_corner.x, bot_right_corner.y);

  return {top_left_corner, top_right_corner, bot_right_corner, bot_left_corner};
}

double RigidBody::CalculateRotationalInertia() {
  ASSERT(m_Type == Box || m_Type == Circle, "m_Type isn't defined");
  ASSERT(m_Size.x > 0.0 || m_Size.y > 0.0, "m_Size isn't initialized");
  ASSERT(m_Mass > 0.0, "m_Mass isn't initialized");
  if (m_Type == Circle) {
    const double radius = m_Size.x;
    return 0.5 * m_Mass * radius * radius;
  }
  const double width = m_Size.x;
  const double height = m_Size.y;
  return 0.083 * m_Mass * (width * width + height * height);
}
} // namespace pe2d
