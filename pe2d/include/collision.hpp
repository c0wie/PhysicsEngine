#pragma once

// local
#include "collision_body.hpp"
#include "collision_points.hpp"

// std
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
  Collision(CollisionBody *a, CollisionBody *b, const CollisionPoints &points)
      : m_Points(points), m_ObjectA(a), m_ObjectB(b) {}

public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Get the first colliding RigidBody.
  /// @return Mutable reference to m_ObjectA.
  /// @throw std::logic_error if the m_ObjectA was destroyed.
  /// @warning The reference becomes invalid if the original RigidBody is destroyed.
  /////////////////////////////////////////////////////////////////////
  CollisionBody *GetObjectA() { 
    if(m_ObjectA == nullptr) {
      throw std::logic_error("m_ObjectA is nullptr");
    }
    return m_ObjectA; 
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Get the second colliding RigidBody.
  /// @return Mutable reference to m_ObjectB.
  /// @throw std::logic_error if the m_ObjectB was destroyed.
  /// @warning The reference becomes invalid if the original RigidBody is destroyed.
  /////////////////////////////////////////////////////////////////////
  CollisionBody *GetObjectB() {
    if(m_ObjectB == nullptr) {
      throw std::logic_error("m_ObjectB is nullptr");
    } 
    return m_ObjectB; 
  }
  
  /////////////////////////////////////////////////////////////////////
  /// @brief Gets the collision details.
  /// @return Mutable reference to the collision points information.
  /////////////////////////////////////////////////////////////////////
  CollisionPoints &GetCollisionPoints() { return m_Points; }
private:
  CollisionPoints m_Points; ///< Detailed collision information
  CollisionBody *m_ObjectA{nullptr}; ///< Non-owning pointer to first colliding body
  CollisionBody *m_ObjectB{nullptr}; ///< Non-owning pointer to second colliding body
};
} // namespace pe2d