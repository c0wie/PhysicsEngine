#pragma once

// local
#include "scene_settings.hpp"

// lib
// pe2d
#include "rigid_body.hpp"
#include "collision.hpp"
#include <cstddef>
#define private public
#include "physics_world.hpp"
#define private private
// SFML
#include <SFML/Graphics.hpp>

// std
#include <vector>

struct Entity {
  sf::Color color;
  pe2d::RigidBody core;
};

class GameWorld {
public:
  GameWorld(unsigned int substeps);
  void Update(sf::Vector2i position, float delta_time);
  void Draw(sf::RenderWindow &window, const SceneSettings &scene_settings);
  void AddEntity(const Entity &entity);
  void Clear();
  void RemoveGrid() {m_PhysicsWorld.RemoveGrid();}
  void AddGrid(pe2d::Vec2f top_left_corner, int rows, int columns, float cell_size) {
    m_PhysicsWorld.AddGrid(top_left_corner, rows, columns, cell_size);
  }
  void SetSolver(std::function<void(std::vector<pe2d::Collision> &collisions)> solver) {
    m_PhysicsWorld.SetSolver(solver);
  }
  Entity GetLastEntity() const {return m_Entities.back();}
  std::size_t Size() const {return m_Entities.size();}
private:
  void SetUp();
private:
  constexpr static float SPAWN_COOLDOWN = 0.1f;
  float m_LastSpawnTime;
  std::vector<Entity> m_Entities;
  pe2d::PhysicsWorld m_PhysicsWorld;
};