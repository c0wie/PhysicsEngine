#pragma once

// local
#include "collision_world.hpp"
#include "rigid_body.hpp"

// std
#include <cstddef>

namespace pe2d {
/*
  The PhysicsWorld class is the central component of the pe2d physics library.
   It manages all physics objects and handles the entire physics simulation
   process. PhysicsWorld is designed to remind std::unordered_map, providing
   key-value operations and iterators for easy management of physics objects.

    To use the PhysicsWorld class effectively, you must add a physics solver.
   The library provides two solvers, but custom solvers can be implemented by
   creating a function wich returns void and has to parameters
    std::vector<pe2d::Collision> &collisions and float deltaTime.

    PhysicsWorld also supports broad-phase collision detection using a spatial
   partitioning technique called a Grid. The Grid can be activated by calling
   the appropriate function, and it helps to optimize collision detection with
   bigger simulations by dividing the simulation space into smaller regions.
*/
class PhysicsWorld : public CollisionWorld{
public:
  PhysicsWorld() = default;
  explicit PhysicsWorld(int substeps);
  PhysicsWorld(const PhysicsWorld &other) = delete;
  PhysicsWorld &operator=(const PhysicsWorld &other) = delete;
  PhysicsWorld(PhysicsWorld &&other) = default;
  PhysicsWorld &operator=(PhysicsWorld &&other) = default;

public:
  void AddRigidBody(RigidBody *object);
  void RemoveRigidBody(RigidBody *object); 
  void Step(float delta_time);
private:
  void ApplyGravity();
  void MoveObjects(float delta_time);
};
} // namespace pe2d