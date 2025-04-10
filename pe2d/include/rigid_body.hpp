#pragma once

// local
#include "algo.hpp"
#include "angle.hpp"
#include "collision_body.hpp"
#include "vector2.hpp"
// std
#include <stdexcept>
#include <string>

namespace pe2d {

/////////////////////////////////////////////////////////////////////
/// @brief Object which can't be deformated with 3 degrees of freedom and basic physcis properties.
/////////////////////////////////////////////////////////////////////
class RigidBody : public CollisionBody {
public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Default constructor sets all members to 0.
  /////////////////////////////////////////////////////////////////////
  RigidBody() = default;

  /////////////////////////////////////////////////////////////////////
  /// @brief Constructor of object without friction and initial velocity set to 0.
  /// @param type - Geometry type (circle or box)
  /// @param size - Object dimensions (must be positive)
  /// @param transform - Object size, angle and scale
  /// @param mass - Object mass (must be positive)
  /// @param is_static - If true, object won't respond to forces
  /// @param gravity - Force which would push object in selected direction
  /// @throw std::invalid_argument if size is non positive in any axis
  /// @throw std::invalid_argument if mass is non positive (only for non-static objects)
  /// @note For static objects, mass and velocity parameters are ignored.
  /////////////////////////////////////////////////////////////////////
  RigidBody(BodyType type, Vec2f size, Transform transform, float mass,
            bool is_static, Vec2f gravity);

  /////////////////////////////////////////////////////////////////////
  /// @brief Constructor of object without friction and with initial velocity set.
  /// @param type - Geometry type (circle or box)
  /// @param size - Object dimensions (must be positive)
  /// @param transform - Object size, angle and scale
  /// @param mass - Object mass (must be positive)
  /// @param is_static - If true, object won't respond to forces
  /// @param gravity - Constant force applied in each frame
  /// @param linear_velocity - Initial movement speed 
  /// @param angular_velocity - Initial rotation speed (radians/sec)
  /// @throw std::invalid_argument if size is non positive in any axis
  /// @throw std::invalid_argument if mass is non positive (only for non-static objects)
  /// @note For static objects, mass and velocity parameters are ignored.
  /// @see RigidBodyType
  /// @see Transform
  /////////////////////////////////////////////////////////////////////
  RigidBody(BodyType type, Vec2f size, Transform transform, float mass,
            bool is_static, Vec2f gravity, Vec2f linear_velocity,
            float angular_velocity);

  /////////////////////////////////////////////////////////////////////
  /// @brief Constructor of object with friction and with initial velocity set.
  /// @param type - Geometry type (circle or box)
  /// @param size - Object dimensions (must be positive)
  /// @param transform - Object size, angle and scale
  /// @param mass - Object mass (must be positive)
  /// @param is_static - If true, object won't respond to forces
  /// @param gravity - Constant force applied in each frame
  /// @param linear_velocity - Initial movement speed 
  /// @param angular_velocity - Initial rotation speed (radians/sec)
  /// @param static_friction - Friction scalar used when object is not moving
  /// @param dynamic_friction - Friction scalar used when object is moving
  /// @param restitution - Scalar defininf how bouncy objellt would be  
  /// @throw std::invalid_argument if size is non positive in any axis
  /// @throw std::invalid_argument if mass is non positive (only for non-static objects)
  /// @throw std::invalid_argument if static_friction is not in range [0.0 - 1.0]
  /// @throw std::invalid_argument if dynamic_friction is not in range [0.0 - 1.0]
  /// @throw std::invalid_argument if restitution is not in range [0.0 - 1.0]
  /// @note For static objects, mass and velocity parameters are ignored.
  /// @see RigidBodyType
  /// @see Transform
  /////////////////////////////////////////////////////////////////////
  RigidBody(BodyType type, Vec2f size, Transform transform, float mass,
            bool is_static, Vec2f gravity, Vec2f linear_velocity,
            float angular_velocity, float static_friction,
            float dynamic_friction, float restitution);

public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the scale of the rigid body.
  /// @return The scale vector(x - scale in x axis, y - scale in y axis).
  /////////////////////////////////////////////////////////////////////
  constexpr Vec2f GetScale() const { return m_Transform.scale; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the angle of the rigid body.
  /// @return The angle object.
  /// @see Angle
  /////////////////////////////////////////////////////////////////////
  constexpr Angle GetAngle() const { return m_Transform.angle; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the transform of the rigid body.
  /// @return The transform object.
  /// @see Transform
  /////////////////////////////////////////////////////////////////////
  constexpr Transform GetTransform() const { return m_Transform; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the mass of the rigid body
  /// @return The mass value.
  /// @retval math::INF If the body is static
  /// @note Static bodies have infinite mass to prevent movement
  /////////////////////////////////////////////////////////////////////
  constexpr float GetMass() const { return m_Mass; }
  
  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the mass inverse of the rigid body.
  /// @return The mass inverse value.
  /// @retval 0.0 if the body is static.
  /////////////////////////////////////////////////////////////////////
  constexpr float GetInvMass() const {
    if (m_IsStatic) {
      return 0.0f;
    }
    return (1.0f / m_Mass);
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the force acting on rigid body.
  /// @return The force vector.
  /////////////////////////////////////////////////////////////////////
  constexpr Vec2f GetForce() const { return m_Force; }
  
  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the gravity assigned to rigid body.
  /// @return The gravity vector.
  /////////////////////////////////////////////////////////////////////
  constexpr Vec2f GetGravity() const { return m_Gravity; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the angular velocity of rigid body.
  /// @return The angular velocity value.
  /// @note In 2D angular velocity is not a vector because its direction is
  ///       going into third dimension.
  /////////////////////////////////////////////////////////////////////
  constexpr float GetAngularVelocity() const { return m_AngularVelocity; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the rotational inertia of rigid body.
  /// @return The rotational inertia value.
  /// @retval math::INF If body is static.
  /////////////////////////////////////////////////////////////////////
  constexpr float GetRotationalInertia() const { return m_RotationalInertia; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the rotational inertia of rigid body.
  /// @return The rotational inertia value.
  /// @retval 0.0 If body is static.
  /////////////////////////////////////////////////////////////////////
  constexpr float GetInvRotationalInertia() const {
    if (m_IsStatic) {
      return 0.0f;
    }
    return 1.0f / m_RotationalInertia;
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the static friction of rigid body.
  /// @return Static friction value.
  /////////////////////////////////////////////////////////////////////
  constexpr float GetStaticFriction() const { return m_StaticFriction; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the dynamic friction of rigid body.
  /// @return Dynamic friction value.
  /////////////////////////////////////////////////////////////////////
  constexpr float GetDynamicFriction() const { return m_DynamicFriction; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the restitution of rigid body.
  /// @return Restitution value.
  /////////////////////////////////////////////////////////////////////
  constexpr float GetRestitution() const { return m_Restitution; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Sets size of rigid body.
  /// @details When rigid body type is circle, size.x is assigned to both
  /// components.
  ///          Resizing the rigid body affects its rotational inertia.
  /// @param size - new size of rigid body
  /// @throw std::invalid_argument for BOX if any component is non positive
  ///        for CIRCLE if x component is non positive.
  /////////////////////////////////////////////////////////////////////
  void SetSize(Vec2f size) {
    ASSERT(m_Type == Box || m_Type == Circle, "m_Type isn't defined");
    if (size.x <= 0) {
      throw std::invalid_argument(
          "[RigidBody::SetSize()] Error: size.x must be positive (received: " +
          size.GetString() + ")");
    }
    if (m_Type == Box) {
      if (size.y <= 0) {
        throw std::invalid_argument("[RigidBody::SetSize()] Error: size.y must "
                                    "be positive (received: " +
                                    size.GetString() + ")");
      }
      m_Size = size;
    } else if (m_Type == Circle) {
      // radius
      m_Size = Vec2f(size.x, size.x);
    }
    m_RotationalInertia = algo::CalculateRotationalInertia(m_Type, m_Size, m_Mass);
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Sets scale of rigid body.
  /// @details When rigid body type is circle, scale.x is assigned to both
  /// components.
  ///          Rescaling the rigid body does not affect its rotational inertia.
  /// @param scale - new scale of the rigid body
  /// @throw std::invalid_argument for BOX if any component is non positive
  ///        for CIRCLE if x component is non positive.
  /////////////////////////////////////////////////////////////////////
  constexpr void SetScale(Vec2f scale) {
    ASSERT(m_Type == Box || m_Type == Circle, "m_Type isn't defined");
    if (scale.x <= 0.0) {
      throw std::invalid_argument("[RigidBody::SetScale()] Error: scale.x must "
                                  "be positive (received: " +
                                  scale.GetString() + ")");
    }
    if (m_Type == Box) {
      if (scale.y <= 0.0) {
        throw std::invalid_argument("[RigidBody::SetScale()] Error: scale.y "
                                    "must be positive (received: " +
                                    scale.GetString() + ")");
      }
      m_Transform.scale = scale;
    } else if (m_Type == Circle) {
      m_Transform.scale = Vec2f(scale.x, scale.x);
    }
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Sets new angle to rigid body.
  /// @param angle - new angle of rigid body
  /////////////////////////////////////////////////////////////////////
  constexpr void SetAngle(Angle angle) { m_Transform.angle = angle; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Adds angle to currnet rigid body rotation.
  /// @param angle - angle rigid body is rotated
  /////////////////////////////////////////////////////////////////////
  constexpr void Rotate(Angle angle_offset) { m_Transform.Rotate(angle_offset); }

  /////////////////////////////////////////////////////////////////////
  /// @brief Sets transform of rigid body.
  /// @param transform - sets new rigid body transform
  /// @throw std::invalid_argument if any of transform.scale components is non positive. 
  /// @see SetPosition
  /// @see SetAngle
  /// @see SetScale
  /////////////////////////////////////////////////////////////////////
  constexpr void SetTransform(Transform transform) {
    SetPosition(transform.position);
    SetAngle(transform.angle);
    SetScale(transform.scale);
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Sets mass of rigid body.
  /// @details Do nothing if object is static. Change in mass affects 
  ///          rigid body rotational inertia.
  /// @param mass - new mass of rigid body
  /// @throw std::invalid_argument if mass is not positive.
  /// @see CalculateRotationalInertia
  /////////////////////////////////////////////////////////////////////
  void SetMass(float mass) {
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
    m_RotationalInertia = algo::CalculateRotationalInertia(m_Type, m_Size, m_Mass);
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Sets force acting on rigid body.
  /// @param force - new force acting on rigid body
  /////////////////////////////////////////////////////////////////////
  constexpr void SetForce(Vec2f force) { m_Force = force; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Adds force acting on rigid body.
  /// @param force - force added to act on rigid body 
  /////////////////////////////////////////////////////////////////////
  constexpr void AddForce(Vec2f force) { m_Force += force; }
  
  /////////////////////////////////////////////////////////////////////
  /// @brief Sets gravity assigned to rigid body.
  /// @param gravity - new gravity assigned to rigid body
  /////////////////////////////////////////////////////////////////////
  constexpr void SetGravity(Vec2f gravity) { m_Gravity = gravity; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Sets angular velocity of rigid body.
  /// @param angular_velocity - new angular velocity of rigid body
  /////////////////////////////////////////////////////////////////////
  constexpr void SetAngularVelocity(float angular_velocity) {
    m_AngularVelocity = angular_velocity;
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Adds angular velocity to currnet rigid body angular velocity.
  /// @param angular_veloctiy - angular velocity added to rigid body 
  /////////////////////////////////////////////////////////////////////
  constexpr void AddAngularVelocity(float angular_velocity) {
    m_AngularVelocity += angular_velocity;
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Sets static friction of rigid body.
  /// @param static_friction - new static friction of rigid body
  /// @throws std::invalid_argument if new static friction is not in range [0.0 - 1.0] 
  /////////////////////////////////////////////////////////////////////
  constexpr void SetStaticFriction(float static_friction) {
    if (static_friction > 1.0 || static_friction < 0.0) {
      throw std::invalid_argument(
          "[RigidBody::SetStaticFriction()] Error: static friction must be "
          "beetwen [0.0 - 1.0] (received: " +
          std::to_string(static_friction) + ")");
    }
    m_StaticFriction = static_friction;
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Sets dynamic friction of rigid body.
  /// @param dynamic_friction - new dynamic friction of rigid body
  /// @throws std::invalid_argument if new dynamic friction is not in range [0.0 - 1.0] 
  /////////////////////////////////////////////////////////////////////
  constexpr void SetDynamicFriction(float dynamic_friction) {
    if (dynamic_friction > 1.0 || dynamic_friction < 0.0) {
      throw std::invalid_argument(
          "[RigidBody::SetDynamicFriction()] Error: dynamic friction must be "
          "beetwen [0.0 - 1.0] (received: " +
          std::to_string(dynamic_friction) + ")");
    }
    m_DynamicFriction = dynamic_friction;
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Sets restitution of rigid body.
  /// @param restitution - new restitution of rigid body
  /// @throws std::invalid_argument if new restitution is not in range [0.0 - 1.0] 
  /////////////////////////////////////////////////////////////////////
  constexpr void SetRestitution(float restitution) {
    if (restitution > 1.0 || restitution < 0.0) {
      throw std::invalid_argument(
          "[RigidBody::SetRestitution()] Error: restitution must be beetwen "
          "[0.0 - 1.0] (received: " +
          std::to_string(restitution) + ")");
    }
    m_Restitution = restitution;
  }
private:
  Vec2f m_Gravity{0.0f, 0.0f};
  Vec2f m_Force{0.0f, 0.0f};
  float m_Mass{0.0f};
  float m_AngularVelocity{0.0f};
  float m_RotationalInertia{0.0f};
  // Static friction coefficient [in range 0.0 - 1.0]
  float m_StaticFriction{0.0f};
  // Dynamic friction coefficient [in range 0.0 - 1.0]
  float m_DynamicFriction{0.0f};
  // Elasticy of collision [in range 0.0 - 1.0]
  float m_Restitution{0.0f};
};
} // namespace pe2d