//header
#include "collision_world.hpp"
#include "collider.hpp"
#include "collision_body.hpp"

namespace pe2d {
using iterator = std::unordered_map<size_t, CollisionBody *>::iterator;
using const_iterator =
    std::unordered_map<size_t, CollisionBody *>::const_iterator;

void CollisionWorld::Step(float delta_time) {
  if (delta_time <= 0.0f) {
    throw std::invalid_argument("[CollisionWorld::Step()] Error: delta_time must "
                                "be positive (received: " +
                                std::to_string(delta_time) + ")");
  }
  for (std::size_t i = 0; i < m_Substeps; i++) {
    const float subtime = delta_time / (float)m_Substeps;
    ApplyGravity();
    ResolveCollisions(subtime);
    MoveObjects(subtime);
  }
}

void CollisionWorld::AddObject(CollisionBody *object, std::size_t id) {
  if(m_Objects.find(id) != m_Objects.end()) {
    throw std::invalid_argument("[CollisionWorld::AddObject()] Error: each "
                                "object must have a unique id(received:" +
                                std::to_string(id) + ")");
  }
  m_Objects[id] = object;
}

iterator CollisionWorld::RemoveObject(size_t id) {
  iterator it = m_Objects.find(id);
  if (it != m_Objects.end()) {
    return m_Objects.erase(it);
  }
  return m_Objects.end();
}

CollisionBody * CollisionWorld::At(unsigned int id) {
  if (m_Objects.find(id) == m_Objects.end()) {
    throw std::invalid_argument("[CollisionWorld::At()] Error: no object exists"
                                "with the given id (received " +
                                std::to_string(id) + ")");
  }
  return m_Objects.at(id);
}

void CollisionWorld::ResolveCollisions(float delta_time) {
  std::vector<Collision> collisions;
  collisions.reserve(m_Objects.size());
  if (m_IsGridOn) {
    m_Grid.Update(m_Objects);
    std::list<std::pair<size_t, size_t>> pairs = m_Grid.GetCollisionPairs();
    for (auto it = pairs.begin(); it != pairs.end(); it++) {
      FindCollisions(it->first, it->second, collisions);
    }
  } else {
    for (auto itA = m_Objects.begin(); itA != m_Objects.end(); itA++) {
      for (auto itB = m_Objects.begin(); itB != m_Objects.end(); itB++) {
        if (itA == itB) {
          break;
        }
        FindCollisions(itA->first, itB->first, collisions);
      }
    }
  }
  m_Solver(collisions);
}

void CollisionWorld::RemoveGrid() {
  if (!m_IsGridOn) {
    return;
  }
  m_Grid = Grid();
  m_IsGridOn = false;
}

void CollisionWorld::ResizeGrid(Vec2f top_left_corner, int rows, int columns,
                              float cell_size) {
  m_Grid = Grid(top_left_corner, rows, columns, cell_size);
}

void CollisionWorld::FindCollisions(size_t idA, size_t idB,
                                  std::vector<Collision> &collisions) {
  CollisionBody *a = m_Objects.at(idA);
  CollisionBody *b = m_Objects.at(idB);
  if (a->IsStatic() && b->IsStatic()) {
    return;
  }
  // check if objects are penetrating if so pull them apart
  const CollisionPoints points = TestCollison(a, b);
  if (points.contact_count != 0) {
    collisions.emplace_back(a, b, points);
  }
}

void CollisionWorld::MoveObjects(float delta_time) {
  for (auto it = Begin(); it != End(); it++) {
    CollisionBody *object = it->second;
    if (object->IsStatic()) {
      continue;
    }
    object->Move(object->GetLinearVelocity() * delta_time);
  }
}
} //namespace pe2d