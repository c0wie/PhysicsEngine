#pragma once

//local
#include "vector2.hpp"
#include "transform.hpp"
#include "assert.hpp"
#include "algo.hpp"

// std
#include <array>

enum BodyType : int { Box = 1, Circle = 2 };

namespace pe2d {
  class CollisionBody {
  public:
    CollisionBody() = default;
    CollisionBody(BodyType type, Vec2f size, Vec2f position, Vec2f gravity, Vec2f velocity, bool is_static) :
      m_Type(type), m_Transform(position), m_Gravity(gravity), m_LinearVelocity(velocity), m_IsStatic(is_static) {
      SetSize(size);
    }
    CollisionBody(BodyType type, Vec2f size, Vec2f position, Vec2f gravity, bool is_static) :
      m_Type(type), m_Transform(position), m_Gravity(gravity), m_IsStatic(is_static) {
    }
  public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the body type of the rigid body.
  /// @return The body type.
  /// @retval Circle 
  /// @retval Box
  /////////////////////////////////////////////////////////////////////
  constexpr BodyType GetType() const { return m_Type; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the size of the rigid body.
  /// @details When rigid body type is circle the x and y components are the same.
  /// @return The size vector(x - size in x axis, y - size in y axis).
  /////////////////////////////////////////////////////////////////////
  constexpr Vec2f GetSize() const { return m_Size; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Creates the smallest non-rotated box containing body.
  /// @return Array of vertices starting on top left corner an going
  ///         counter clockwise.
  /////////////////////////////////////////////////////////////////////
  std::array<Vec2f, 4> GetBoundingBox() const {
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

  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the position of the rigid body.
  /// @return The position vector.
  /// @note Position is defined in geometrical center of rigid body.
  /////////////////////////////////////////////////////////////////////
  constexpr Vec2f GetPosition() const { return m_Transform.position; }
  
  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the linear velocity of rigid body.
  /// @return The linear velocity vector.
  /////////////////////////////////////////////////////////////////////
  constexpr Vec2f GetLinearVelocity() const { return m_LinearVelocity; }

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
  /// @brief Checks if the body is static.
  /// @return True if the body is static, false if dynamic.
  /////////////////////////////////////////////////////////////////////
  constexpr bool IsStatic() const { return m_IsStatic; }
  
  /////////////////////////////////////////////////////////////////////
  /// @brief Sets size of rigid body.
  /// @details When rigid body type is circle, size.x is assigned to both
  /// components.
  ///          Resizing the rigid body affects its rotational inertia.
  /// @param size - new size of rigid body
  /// @throw std::invalid_argument for Box if any component is non positive
  ///        for Circle if x component is non positive.
  /////////////////////////////////////////////////////////////////////
  void SetSize(Vec2f size) {
    ASSERT(m_Type == Box || m_Type == Circle, "m_Type is not defined");
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
      m_Size = Vec2f(size.x, size.x);
    }
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Sets position of rigid body.
  /// @param pos - new position of rigid body
  /////////////////////////////////////////////////////////////////////
  constexpr void SetPosition(Vec2f pos) { m_Transform.position = pos; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Adds offset to current rigid body position. 
  /// @param offset - distance rigid body is be moved 
  /////////////////////////////////////////////////////////////////////
  constexpr void Move(Vec2f offset) { m_Transform.Move(offset); }

  /////////////////////////////////////////////////////////////////////
  /// @brief Sets linear velocity of rigid body.
  /// @param linear_velocity- new linear velocity of rigid body
  /////////////////////////////////////////////////////////////////////
  constexpr void SetLinearVelocity(Vec2f linear_velocity) {
    m_LinearVelocity = linear_velocity;
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Adds linear velocity to currnet rigid body linear velocity.
  /// @param linear_veloctiy - linear velocity added to rigid body 
  /////////////////////////////////////////////////////////////////////
  constexpr void AddLinearVelocity(Vec2f linear_velocity) {
    m_LinearVelocity += linear_velocity;
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

  protected:
    bool m_IsStatic;
    Vec2f m_Size{0.0f, 0.0f};
    BodyType m_Type{0};
    Vec2f m_LinearVelocity{0.0f, 0.0f};
    Vec2f m_Force{0.0f, 0.0f};
    Vec2f m_Gravity{0.0f, 0.0f};
    Transform m_Transform;
  };
}