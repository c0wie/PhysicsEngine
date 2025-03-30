#pragma once

// local
#include "rigid_body.hpp"
#include "scene_settings.hpp"

// lib
// pe2d
#include "collision.hpp"
#define private public
#include "physics_world.hpp"
#define private private
// SFML
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

// std
#include <vector>

class VisualWorld {
public:
  VisualWorld(unsigned int substeps);
  void SetUp();
  void Step(float delta_time);
  void Draw(sf::RenderWindow &window, const SceneSettings &scene_settings);
  void Update(sf::Vector2i position, float delta_time);
  pe2d::PhysicsWorld &GetPhysicsWorld() { return m_PhysicsWorld; }
  std::vector<pe2d::Collision> GetCollisions() const { return m_Collisions; }
  pe2d::RigidBody GetLastObject() { return m_PhysicsWorld.At(m_LastId); }

private:
  constexpr static float SPAWN_COOLDOWN = 0.1f;
  float m_LastSpawnTime;
  std::size_t m_LastId = 0;
  pe2d::PhysicsWorld m_PhysicsWorld;
  std::vector<pe2d::Collision> m_Collisions;
};