#pragma once

// local
#include "collision_points.hpp"
#include "rigid_body.hpp"

namespace pe2d {
/*
    Struct holding general information about collision:
    - `ObjectA` and `ObjectB`: objects involved in collision
    - `Points`: sturct with detailed information obout collision
*/
struct Collision {
public:
  Collision(RigidBody &a, RigidBody &b, const CollisionPoints &points)
      : m_Points(points), m_ObjectA(&a), m_ObjectB(&b) {}

public:
  RigidBody &GetObjectA() { return *m_ObjectA; }
  RigidBody &GetObjectB() { return *m_ObjectB; }
  CollisionPoints &GetCollisionPoints() { return m_Points; }

private:
  CollisionPoints m_Points;
  RigidBody *m_ObjectA;
  RigidBody *m_ObjectB;
};
} // namespace pe2d