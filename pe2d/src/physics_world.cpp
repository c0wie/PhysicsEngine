// header
#include "physics_world.hpp"

// local
#include "angle.hpp"
#include "collision_world.hpp"
#include "rigid_body.hpp"
#include "solver.hpp"
#include "vector2.hpp"

// std
#include <cstddef>
#include <stdexcept>
#include <string>

namespace pe2d {

PhysicsWorld::PhysicsWorld(int substeps) : CollisionWorld(substeps) {
  SetSolver(ImpulseSolver);
}

void PhysicsWorld::Step(float delta_time) {
  if (delta_time < 0.0f) {
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

void PhysicsWorld::AddRigidBody(RigidBody *object) {
  m_Objects.push_back(object);
}

void PhysicsWorld::RemoveRigidBody(RigidBody *object) {
  m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object));
} 

void PhysicsWorld::ApplyGravity() {
  for (auto object : m_Objects) {
    if (object->IsStatic() || !object->IsDynamic()) {
      continue;
    }
    RigidBody *rb = static_cast<RigidBody *>(object);
    rb->AddForce(rb->GetGravity() * rb->GetMass());
  }
}

void PhysicsWorld::MoveObjects(float delta_time) {
  for(auto object : m_Objects) {
    if (object->IsStatic() || object->IsDynamic() == false) {
      continue;
    }
    RigidBody *rb = static_cast<RigidBody *>(object);
    // linear integration
    const Vector2 acceleration = rb->GetForce() * rb->GetInvMass();
    rb->AddLinearVelocity(acceleration * delta_time * 0.5f);
    rb->Move(rb->GetLinearVelocity() * delta_time);
    rb->AddLinearVelocity(acceleration * delta_time * 0.5f);

    rb->Rotate(
        Angle::FromRadians(rb->GetAngularVelocity() * delta_time));
    rb->SetForce({0.0f, 0.0f});
  }
}
} // namespace pe2d