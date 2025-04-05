// header
#include "physics_world.hpp"
#include "angle.hpp"
#include "collider.hpp"
#include "collision_points.hpp"
#include "rigid_body.hpp"
#include "vector2.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>

namespace pe2d {
void PhysicsWorld::Step(float delta_time) {
  if (delta_time < 0.0) {
    throw std::invalid_argument("[PhysicsWorld::Step()] Error: delta_time must "
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
void PhysicsWorld::AddObject(const RigidBody &object) {
  const std::size_t id = object.GetID();
  if (m_Objects.find(id) != m_Objects.end()) {
    throw std::invalid_argument("[PhysicsWorld::AddObject()] Error: object's "
                                "id must be unique (received:" +
                                std::to_string(id) + ")");
  }
  m_Objects[id] = object;
}
std::unordered_map<size_t, RigidBody>::iterator
PhysicsWorld::RemoveObject(size_t id) {
  iterator it = m_Objects.find(id);
  if (it != m_Objects.end()) {
    return m_Objects.erase(it);
  }
  return m_Objects.end();
}
RigidBody &PhysicsWorld::At(unsigned int id) {
  if (m_Objects.find(id) == m_Objects.end()) {
    throw std::invalid_argument("[PhysicsWorld::At()] Error: object with given "
                                "id doesn't exist (received " +
                                std::to_string(id) + ")");
  }
  return m_Objects.at(id);
}
void PhysicsWorld::ResolveCollisions(float delta_time) {
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

void PhysicsWorld::AddGrid(Vec2f top_left_corner, Vec2i size,
                           float cell_size) {
  if (m_IsGridOn) {
    return;
  }
  m_Grid = Grid(top_left_corner, size, cell_size);
  m_IsGridOn = true;
}
void PhysicsWorld::RemoveGrid() {
  if (!m_IsGridOn) {
    return;
  }
  m_Grid = Grid();
  m_IsGridOn = false;
}
void PhysicsWorld::ResizeGrid(Vec2f top_left_corner, Vec2i size,
                              float cell_size) {
  m_Grid = Grid(top_left_corner, size, cell_size);
}
void PhysicsWorld::FindCollisions(size_t idA, size_t idB,
                                  std::vector<Collision> &collisions) {
  RigidBody &a = m_Objects.at(idA);
  RigidBody &b = m_Objects.at(idB);
  if (a.IsStatic() && b.IsStatic()) {
    return;
  }
  // check if objects are penetrating if so pull them apart
  const CollisionPoints points = TestCollison(a, b);
  if (points.contact_count != 0) {
    collisions.emplace_back(a, b, points);
  }
}
void PhysicsWorld::ApplyGravity() {
  for (auto it = m_Objects.begin(); it != m_Objects.end(); it++) {
    RigidBody &object = it->second;
    if (object.IsStatic()) {
      continue;
    }
    object.AddForce(object.GetGravity() * object.GetMass());
  }
}
void PhysicsWorld::MoveObjects(float delta_time) {
  for (auto it = Begin(); it != End(); it++) {
    RigidBody &object = it->second;
    if (object.IsStatic()) {
      continue;
    }
    // linear integration
    const Vector2 acceleration = object.GetForce() * object.GetInvMass();
    object.AddLinearVelocity(acceleration * delta_time * 0.5);
    object.Move(object.GetLinearVelocity() * delta_time);
    object.AddLinearVelocity(acceleration * delta_time * 0.5);

    object.Rotate(Angle::FromRadians(object.GetAngularVelocity() * delta_time));
    object.SetForce(Vec2f(0.0f, 0.0f));
  }
}
} // namespace pe2d