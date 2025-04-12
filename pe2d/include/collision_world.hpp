#pragma once

//local
#include "collision.hpp"
#include "collision_body.hpp"
#include "grid.hpp"
#include "solver.hpp"

//std
#include <functional>
#include <vector>

namespace pe2d {
class CollisionWorld {
public:
  CollisionWorld() = default;
  explicit CollisionWorld(int substeps) : m_Substeps(substeps) {
    if (m_Substeps <= 0 || m_Substeps > 64) {
      std::invalid_argument("[CollisionWorld::CollisionWorld()] Error: substeps "
                            "must be between (0 - 64] (received: " +
                            std::to_string(substeps) + ")");
    }
  }
  CollisionWorld(const CollisionWorld &other) = delete;
  CollisionWorld &operator=(const CollisionWorld &other) = delete;
  CollisionWorld(CollisionWorld &&other) = default;
  CollisionWorld &operator=(CollisionWorld &&other) = default;

public:
  void ResolveCollisions(float delta_time);
  void AddCollisionObject(CollisionBody *object);
  void RemoveCollisionObject(CollisionBody *object);
  
  void SetSolver(std::function<void(std::vector<Collision> &collisions)> solver) {
    m_Solver = solver;
  }
  
  void AddGrid(Vec2f top_left_corner, int rows, int columns, float cell_size);
  void RemoveGrid();
  
  bool IsGridOn() const { return m_IsGridOn; }
  size_t Size() const { return m_Objects.size(); }
  bool Empty() const { return m_Objects.empty(); }
  void Clear() { m_Objects.clear(); }

private:
  void FindCollisions(CollisionBody *a, CollisionBody *b,
                      std::vector<Collision> &collisions);
private:
  bool m_IsGridOn{false};
  unsigned int m_Substeps{1U};
  Grid m_Grid;
  std::function<void(std::vector<Collision> &collisions)>
      m_Solver{PositionSolver};
  std::vector<CollisionBody *> m_Objects;
};
} //namespace pe2d