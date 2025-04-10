#pragma once

// local
#include "grid.hpp"
#include "rigid_body.hpp"
#include "solver.hpp"
#include "vector2.hpp"

// std
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <string>
#include <unordered_map>

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
class PhysicsWorld {
  using iterator = std::unordered_map<size_t, RigidBody *>::iterator;
  using const_iterator =
      std::unordered_map<size_t, RigidBody *>::const_iterator;

public:
  PhysicsWorld() = default;
  explicit PhysicsWorld(int substeps) : m_Substeps(substeps) {
    if (m_Substeps <= 0 || m_Substeps > 64) {
      std::invalid_argument("[PhysicsWorld::PhysicsWorld()] Error: substeps "
                            "must be between (0 - 64] (received: " +
                            std::to_string(substeps) + ")");
    }
  }
  PhysicsWorld(const PhysicsWorld &other) = delete;
  PhysicsWorld &operator=(const PhysicsWorld &other) = delete;
  PhysicsWorld(PhysicsWorld &&other) = default;
  PhysicsWorld &operator=(PhysicsWorld &&other) = default;

public:
  void Step(float delta_time);
  void AddObject(RigidBody *object, std::size_t id);
  iterator RemoveObject(size_t id);
  iterator RemoveObject(iterator object) { return m_Objects.erase(object); }
  iterator RemoveObjects(iterator first_object, iterator last_object) {
    return m_Objects.erase(first_object, last_object);
  }
  void ClearObjects() { m_Objects.clear(); }

  void SetSolver(std::function<void(std::vector<Collision> &collisions)> solver) {
    m_Solver = solver;
  }

  void AddGrid(Vec2f top_left_corner, int rows, int columns, float cell_size);
  void RemoveGrid();
  void ResizeGrid(Vec2f top_left_corner, int rows, int columns,
                  float cell_size);

  bool IsGridOn() const { return m_IsGridOn; }
  size_t Size() const { return m_Objects.size(); }
  bool Empty() const { return m_Objects.empty(); }
  iterator Begin() { return m_Objects.begin(); }
  iterator End() { return m_Objects.end(); }
  const_iterator cBegin() const { return m_Objects.cbegin(); }
  const_iterator cEnd() const { return m_Objects.cend(); }
  RigidBody * At(unsigned int id);

private:
  void ResolveCollisions(float delta_time);
  void FindCollisions(size_t idA, size_t idB,
                      std::vector<Collision> &collisions);
  void ApplyGravity();
  void MoveObjects(float delta_time);

private:
  Grid m_Grid;
  std::function<void(std::vector<Collision> &collisions)>
      m_Solver{ImpulseSolver};
  unsigned int m_Substeps{1U};
  bool m_IsGridOn{false};
  std::unordered_map<size_t, RigidBody *> m_Objects;
};
} // namespace pe2d