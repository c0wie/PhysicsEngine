#pragma once

//local
#include "collision.hpp"
#include "collision_body.hpp"
#include "grid.hpp"
#include "solver.hpp"

//std
#include <functional>
#include <unordered_map>

namespace pe2d {
class CollisionWorld {
  using iterator = std::unordered_map<size_t, CollisionBody *>::iterator;
  using const_iterator =
      std::unordered_map<size_t, CollisionBody *>::const_iterator;

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
  void Step(float delta_time);
  void AddObject(CollisionBody *object, std::size_t id);
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
  CollisionBody * At(unsigned int id);

private:
  void ResolveCollisions(float delta_time);
  void FindCollisions(size_t idA, size_t idB,
                      std::vector<Collision> &collisions);
  void ApplyGravity();
  void MoveObjects(float delta_time);

private:
  Grid m_Grid;
  std::function<void(std::vector<Collision> &collisions)>
      m_Solver{PositionSolver};
  unsigned int m_Substeps{1U};
  bool m_IsGridOn{false};
  std::unordered_map<size_t, CollisionBody *> m_Objects;
};
} //namespace pe2d