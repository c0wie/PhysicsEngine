// header
#include "game_world.hpp"

// local
#include "angle.hpp"
#include "collision.hpp"
#include "collision_body.hpp"
#include "collision_points.hpp"
#include "rigid_body.hpp"
#include "scene_settings.hpp"
#include "solver.hpp"
#include "vector2.hpp"

// lib
// sfml
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>

pe2d::RigidBody GetBox(sf::Vector2i pos) {
  const float mass = 1000.f;
  const pe2d::Transform transform =
      pe2d::Transform(pe2d::Vec2f(pos.x, pos.y));
  const pe2d::Vec2f size = pe2d::Vec2f(50.0, 50.0);
  const float staticFriction = 0.8;
  const float dynamicFriction = 0.7;
  const float restitution = 0.0;
  return pe2d::RigidBody(Box, size, transform, mass, false,
                         pe2d::Vector2(0.0f, 98.1f), {}, 0.0, staticFriction,
                         dynamicFriction, restitution);
}

pe2d::RigidBody GetCircle(sf::Vector2i pos) {
  const pe2d::Transform transform =
      pe2d::Transform(pe2d::Vec2f(pos.x, pos.y));
  const float mass = 1000.0;
  const float radius = 20.0;
  const float staticFriction = 0.8;
  const float dynamicFriction = 0.7;
  const float resistance = 0.0;
  return pe2d::RigidBody(Circle, pe2d::Vec2f(radius, radius),
                         transform, mass, false, pe2d::Vector2(0.0f, 98.1f), {},
                         0.0, staticFriction, dynamicFriction, resistance);
}

sf::Vector2f RotatePoint(const sf::Vector2f &point, const sf::Vector2f &center,
                         pe2d::Angle angle) {
  const float radians = angle.AsRadians();
  const float x = point.x - center.x;
  const float y = point.y - center.y;

  const float x_rotated = x * std::cos(radians) - y * std::sin(radians);
  const float y_rotated = x * std::sin(radians) + y * std::cos(radians);

  return sf::Vector2f(x_rotated + center.x, y_rotated + center.y);
}

void DrawRigidBody(const pe2d::RigidBody body, const sf::Color &color,
                   sf::RenderWindow &window, bool draw_bounding_boxes = false) {
  if (body.GetType() == Box) {
    const pe2d::Vector2 size = body.GetSize() * body.GetScale();
    const pe2d::Vector2 pos = body.GetPosition();

    sf::RectangleShape rec(sf::Vector2f(size.x, size.y));
    rec.setOrigin(rec.getSize() / 2.0f);
    rec.setPosition(sf::Vector2f(pos.x, pos.y));
    rec.setFillColor(sf::Color::Transparent);
    rec.setRotation(sf::Angle(sf::radians(body.GetAngle().AsRadians())));
    rec.setOutlineColor(color);
    rec.setOutlineThickness(1.0f);
    window.draw(rec);

  } else if (body.GetType() == Circle) {
    const float radius = body.GetSize().x;

    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(color);
    circle.setRotation(circle.getRotation());
    circle.setOutlineThickness(1.0f);

    const pe2d::Vector2 position = body.GetPosition();
    circle.setPosition(
        {(float)(position.x - radius), (float)(position.y - radius)});

    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
    line[0].position = sf::Vector2f(position.x, position.y);
    line[0].color = color;
    line[1].position =
        RotatePoint(sf::Vector2f(position.x + radius, position.y),
                    line[0].position, body.GetAngle());
    line[1].color = color;
    window.draw(circle);
    window.draw(line);
  }
  if (draw_bounding_boxes) {
    std::array<pe2d::Vec2f, 4> vertices = body.GetBoundingBox();
    for (int i = 0; i < vertices.size(); i++) {
      const int j = (i + 1) % vertices.size();
      sf::Vector2f vertex1 = sf::Vector2f(vertices[i].x, vertices[i].y);
      sf::Vector2f vertex2 = sf::Vector2f(vertices[j].x, vertices[j].y);
      sf::Vertex line[] = {
          sf::Vertex(vertex1, color),
          sf::Vertex(vertex2, color),
      };
      window.draw(line, 2, sf::PrimitiveType::Lines);
    }
  }
}

GameWorld::GameWorld(unsigned int substeps) : m_PhysicsWorld(substeps) {
  m_PhysicsWorld.AddGrid({0, 0}, 100, 100, 100);
  SetUp();
  m_PhysicsWorld.SetSolver(pe2d::ImpulseSolver);
}

void GameWorld::AddEntity(const Entity &entity) {
  m_Entities.reserve(100);
  m_Entities.push_back(entity);
  m_PhysicsWorld.AddRigidBody(&m_Entities.back().core);
}

void GameWorld::SetUp() {
  AddEntity(Entity(sf::Color::Blue, pe2d::RigidBody(Box, {950.0f, 100.0f},
  {pe2d::Vec2f(500.0f, 800.0f)}, 10.0f, true, {}, {}, 0.0f, 0.8f, 1.0f, 0.0f)));

  AddEntity(Entity(sf::Color::Blue, pe2d::RigidBody(Box, {350.0f, 50.0f},
    {pe2d::Vec2f(250.0f, 450.0f), pe2d::Angle::FromDegrees(45.0f)}, 10.0f, true, {}, {}, 0.0f, 0.8f, 1.0f, 0.0f)));
}

void GameWorld::Clear() {
  m_PhysicsWorld.Clear();
  m_Entities.clear();
  SetUp();
}

void GameWorld::Update(sf::Vector2i position, float delta_time) {
  m_LastSpawnTime += delta_time;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
      m_LastSpawnTime >= SPAWN_COOLDOWN) {
    AddEntity(Entity(sf::Color::White, GetBox(position)));
    m_LastSpawnTime = 0.0f;
  } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) &&
             m_LastSpawnTime >= SPAWN_COOLDOWN) {
    AddEntity(Entity(sf::Color::White, GetCircle(position)));
    m_LastSpawnTime = 0.0f;
  }
  m_PhysicsWorld.Step(delta_time);
}

void GameWorld::Draw(sf::RenderWindow &window, const SceneSettings &scene_settings) {
  for (const auto &entity : m_Entities) {
    DrawRigidBody(entity.core, entity.color, window);
  }
}