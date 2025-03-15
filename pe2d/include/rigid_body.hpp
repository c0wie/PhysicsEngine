#pragma once

// local
#include "angle.hpp"
#include "assert.hpp"
#include "transform.hpp"
#include "vector2.hpp"

// std
#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>

namespace pe2d {

enum RigidBodyType : int { Box = 1, Circle = 2 };

// class that holds information about object with dynamics
class RigidBody {
public:
  RigidBody() = default;
  RigidBody(std::size_t id, RigidBodyType type, Size2d size,
            Transform transform, double mass, bool is_static, Vec2d gravity);
  RigidBody(std::size_t id, RigidBodyType type, Size2d size,
            Transform transform, double mass, bool is_static, Vec2d gravity,
            Vec2d linear_velocity, double angular_velocity);
  RigidBody(std::size_t id, RigidBodyType type, Size2d size,
            Transform transform, double mass, bool is_static, Vec2d gravity,
            Vec2d linear_velocity, double angular_velocity,
            double static_friction, double dynamic_friction,
            double restitution);

public:
  constexpr std::size_t GetID() const { return m_ID; }
  int GetType() const { return m_Type; }
  // Returns collection of four vertices representing corners of a non rotated
  // bounding box.
  constexpr Size2d GetSize() const { return m_Size; }
  std::array<Pos2d, 4> GetBoundingBox() const;
  constexpr Pos2d GetPosition() const { return m_Transform.position; }
  constexpr Vec2d GetScale() const { return m_Transform.scale; }
  constexpr Angle GetAngle() const { return m_Transform.angle; }
  constexpr Transform GetTransform() const { return m_Transform; }
  constexpr double GetMass() const { return m_Mass; }
  constexpr double GetInvMass() const {
    if (m_IsStatic) {
      return 0.0;
    }
    return (1.0 / m_Mass);
  }
  constexpr Vec2d GetLinearVelocity() const { return m_LinearVelocity; }
  constexpr double GetAngularVelocity() const { return m_AngularVelocity; }
  constexpr Vec2d GetForce() const { return m_Force; }
  constexpr Vec2d GetGravity() const { return m_Gravity; }
  constexpr double GetRotationalInertia() const { return m_RotationalInertia; }
  constexpr double GetInvRotationalInertia() const {
    if (m_IsStatic) {
      return 0.0;
    }
    return 1.0 / m_RotationalInertia;
  }
  constexpr bool IsStatic() const { return m_IsStatic; }
  constexpr double GetStaticFriction() const { return m_StaticFriction; }
  constexpr double GetDynamicFriction() const { return m_DynamicFriction; }
  constexpr double GetRestitution() const { return m_Restitution; }

  constexpr void SetSize(Size2d size) {
    ASSERT(m_Type == Box || m_Type == Circle, "m_Type isn't defined");
    if (size.x <= 0 || size.y <= 0) {
      throw std::invalid_argument(
          "[RigidBody::SetSize()] Error: size must be positive (received: " +
          size.GetString() + ")");
    }
    if (m_Type == Box) {
      m_Size = size;
    } else if (m_Type == Circle) {
      // radius
      m_Size = Size2d(size.x, size.x);
    }
  }
  constexpr void SetPosition(Pos2d pos) { m_Transform.position = pos; }
  constexpr void Move(Vec2d offset) { m_Transform.Move(offset); }
  constexpr void SetScale(Vec2d scale) {
    ASSERT(m_Type == Box || m_Type == Circle, "m_Type isn't defined");
    if (scale.x <= 0.0 || scale.y <= 0.0) {
      throw std::invalid_argument(
          "[Transform::Transform()] Error: scale must be positive (received: " +
          scale.GetString() + ")");
    }
    if (m_Type == Box) {
      m_Transform.scale = scale;
    } else if (m_Type == Circle) {
      m_Transform.scale = pe2d::Vec2d(scale.x, scale.x);
    }
  }
  constexpr void SetAngle(Angle angle) { m_Transform.angle = angle; }
  constexpr void Rotate(Angle angle) { m_Transform.Rotate(angle); }
  constexpr void SetTransform(Transform transform) {
    SetPosition(transform.position);
    SetAngle(transform.angle);
    SetScale(transform.scale);
  }
  void SetMass(double mass) {
    if (m_IsStatic) {
      throw std::logic_error("[RigidBody::SetMass()] Error: mass can't be set "
                             "to a static rigid body");
    }
    if (mass <= 0.0) {
      throw std::invalid_argument(
          "[RigidBody::SetMass()] Error: mass must be positive: (received: " +
          std::to_string(mass) + ")");
    }
    m_Mass = mass;
    m_RotationalInertia = CalculateRotationalInertia();
  }
  constexpr void SetLinearVelocity(Vec2d linear_velocity) {
    m_LinearVelocity = linear_velocity;
  }
  constexpr void AddLinearVelocity(Vec2d linear_velocity) {
    m_LinearVelocity += linear_velocity;
  }
  constexpr void SetAngularVelocity(double angular_velocity) {
    m_AngularVelocity = angular_velocity;
  }
  constexpr void AddAngularVelocity(double angular_velocity) {
    m_AngularVelocity += angular_velocity;
  }
  constexpr void SetForce(Vec2d force) { m_Force = force; }
  constexpr void AddForce(Vec2d force) { m_Force += force; }
  constexpr void SetGravity(Vec2d gravity) { m_Gravity = gravity; }
  constexpr void SetStaticFriction(double static_friction) {
    if (static_friction > 1.0 || static_friction < 0.0) {
      throw std::invalid_argument(
          "[RigidBody::SetStaticFriction()] Error: static friction must be "
          "beetwen [0.0 - 1.0] (received: " +
          std::to_string(static_friction) + ")");
    }
    m_StaticFriction = static_friction;
  }
  constexpr void SetDynamicFriction(double dynamic_friction) {
    if (dynamic_friction > 1.0 || dynamic_friction < 0.0) {
      throw std::invalid_argument(
          "[RigidBody::SetDynamicFriction()] Error: dynamic friction must be "
          "beetwen [0.0 - 1.0] (received: " +
          std::to_string(dynamic_friction) + ")");
    }
    m_DynamicFriction = dynamic_friction;
  }
  constexpr void SetRestitution(double restitution) {
    if (restitution > 1.0 || restitution < 0.0) {
      throw std::invalid_argument(
          "[RigidBody::SetRestitution()] Error: restitution must be beetwen "
          "[0.0 - 1.0] (received: " +
          std::to_string(restitution) + ")");
    }
    m_Restitution = restitution;
  }

private:
  double CalculateRotationalInertia();

private:
  bool m_IsStatic{false};
  std::size_t m_ID{0U};
  double m_Mass{0.0};
  double m_AngularVelocity{0.0};
  double m_RotationalInertia{0.0};
  // Static friction coefficient [in range 0 - 1]
  double m_StaticFriction{0.0};
  // Dynamic friction coefficient [in range 0 - 1]
  double m_DynamicFriction{0.0};
  // Elasticy of collision [in range 0 - 1]
  double m_Restitution{0.0};
  Size2d m_Size{0.0, 0.0};
  RigidBodyType m_Type{0};
  Vec2d m_LinearVelocity{0.0, 0.0};
  Vec2d m_Force{0.0, 0.0};
  Vec2d m_Gravity{0.0, 0.0};
  Transform m_Transform;
};
} // namespace pe2d