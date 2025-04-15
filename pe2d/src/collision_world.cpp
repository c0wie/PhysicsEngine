//header
#include "collision_world.hpp"
#include "collider.hpp"
#include "collision_body.hpp"
#include "collision_points.hpp"
#include <algorithm>

namespace pe2d {

CollisionWorld::CollisionWorld(int substeps) : m_Substeps(substeps) {
  if (m_Substeps <= 0 || m_Substeps > 64) {
    std::invalid_argument("[CollisionWorld::CollisionWorld()] Error: substeps "
                          "must be between (0 - 64] (received: " +
                          std::to_string(substeps) + ")");
  }
}

void CollisionWorld::AddCollisionObject(CollisionBody *object) {
  m_Objects.push_back(object);
}

void CollisionWorld::RemoveCollisionObject(CollisionBody *object) {
  m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object));
}

void CollisionWorld::ResolveCollisions(float delta_time) {
  std::vector<Collision> collisions;
  collisions.reserve(m_Objects.size());
  if (m_IsGridOn) {
    m_Grid.Update(m_Objects);
    const auto pairs = m_Grid.GetCollisionPairs();
    for (auto it = pairs.begin(); it != pairs.end(); it++) {
      FindCollisions(it->first, it->second, collisions);
    }
  } else {
    for(auto objA : m_Objects) {
      for(auto objB : m_Objects) {
        if(objA == objB) {
          break;
        }
        FindCollisions(objA, objB, collisions);
      }
    }
  }
  m_Solver(collisions);
}

void CollisionWorld::AddGrid(Vec2f top_left_corner, int rows, int columns, float cell_size) {
  m_Grid = Grid(top_left_corner, rows, columns, cell_size);
  m_IsGridOn = true;
}

void CollisionWorld::RemoveGrid() {
  m_Grid = Grid();
  m_IsGridOn = false;
}

void CollisionWorld::FindCollisions(CollisionBody *a, CollisionBody *b,
                                  std::vector<Collision> &collisions) {
  if (a->IsStatic() && b->IsStatic()) {
    return;
  }
  // check if objects are penetrating if so pull them apart
  const CollisionPoints points = TestCollison(a, b);
  if (points.contact_count != 0) {
    collisions.emplace_back(a, b, points);
  }
}

} //namespace pe2d