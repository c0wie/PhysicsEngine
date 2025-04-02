#pragma once

// local
#include "collision_points.hpp"
#include "rigid_body.hpp"
#include <stdexcept>

namespace pe2d {

/////////////////////////////////////////////////////////////////////
/// @brief Struct representing a collision between two RigidBodies.
/// @details Contains collision information and non-owning pointers to the colliding bodies.
///          The caller is responsible for ensuring the RigidBodies outlive this Collision object.
/// @warning Storing Collision objects beyond the lifetime of the RigidBodies will lead to dangling pointers.
/////////////////////////////////////////////////////////////////////
struct Collision {
public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Default constructor is deleted.
  /// @details A Collision must always reference valid RigidBodies.
  /////////////////////////////////////////////////////////////////////
  Collision() = delete;

  /////////////////////////////////////////////////////////////////////
  /// @brief Constructs Collision with bodies a and b with CollisionPoints points.
  /// @param a - First colliding RigidBody (must remain valid)
  /// @param b - Second colliding RigidBody (must remain valid)
  /// @param points - CollisionPoints specifing how object a and b are colliding
  /////////////////////////////////////////////////////////////////////
  Collision(RigidBody &a, RigidBody &b, const CollisionPoints &points)
      : m_Points(points), m_ObjectA(&a), m_ObjectB(&b) {}

public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Get the first colliding RigidBody.
  /// @return Mutable reference to m_ObjectA.
  /// @throw std::logic_error if the m_ObjectA was destroyed.
  /// @warning The reference becomes invalid if the original RigidBody is destroyed.
  /////////////////////////////////////////////////////////////////////
  RigidBody &GetObjectA() { 
    if(m_ObjectA == nullptr) {
      throw std::logic_error("m_ObjectA is nullptr");
    }
    return *m_ObjectA; 
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Get the second colliding RigidBody.
  /// @return Mutable reference to m_ObjectB.
  /// @throw std::logic_error if the m_ObjectB was destroyed.
  /// @warning The reference becomes invalid if the original RigidBody is destroyed.
  /////////////////////////////////////////////////////////////////////
  RigidBody &GetObjectB() {
    if(m_ObjectB == nullptr) {
      throw std::logic_error("m_ObjectB is nullptr");
    } 
    return *m_ObjectB; }
  
  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the collision details.
  /// @return Mutable reference to the collision points information.
  /////////////////////////////////////////////////////////////////////
  CollisionPoints &GetCollisionPoints() { return m_Points; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Get the first colliding RigidBody.
  /// @return Const reference to m_ObjectA.
  /// @throw std::logic_error if the m_ObjectA was destroyed.
  /// @warning The reference becomes invalid if the original RigidBody is destroyed.
  /////////////////////////////////////////////////////////////////////
  const RigidBody &GetObjectA() const { 
    if(m_ObjectA == nullptr) {
      throw std::logic_error("m_ObjectA is nullptr");
    }
    return *m_ObjectA; 
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Get the second colliding RigidBody.
  /// @return Const reference to m_ObjectB.
  /// @throw std::logic_error if the m_ObjectB was destroyed.
  /// @warning The reference becomes invalid if the original RigidBody is destroyed.
  /////////////////////////////////////////////////////////////////////
  const RigidBody &GetObjectB() const {
    if(m_ObjectB == nullptr) {
      throw std::logic_error("m_ObjectB is nullptr");
    } 
    return *m_ObjectB; }
  
  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the collision details.
  /// @return Const reference to the collision points information.
  /////////////////////////////////////////////////////////////////////
  const CollisionPoints &GetCollisionPoints() const { return m_Points; }

private:
  CollisionPoints m_Points; ///< Detailed collision information
  RigidBody *m_ObjectA{nullptr}; ///< Non-owning pointer to first colliding body
  RigidBody *m_ObjectB{nullptr}; ///< Non-owning pointer to second colliding body
};
} // namespace pe2d