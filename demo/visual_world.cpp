// header
#include "visual_world.hpp"

// local
#include "assert.hpp"
#include "collision.hpp"
#include "scene_settings.hpp"
#include "solver.hpp"
#include "vector2.hpp"

// lib
// sfml
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

// std
#include <random>

// returns box with randomized parameters excluding frictions and restitution
pe2d::RigidBody GetRandomBox(size_t id, sf::Vector2i pos) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribInt(30, 150);
  std::uniform_real_distribution<float> distribFloat(0.0, 1.0);
  std::uniform_int_distribution<int> distribPos(200, 800);
  std::uniform_int_distribution<int> distribRotation(0, 180);
  float mass = 1000.f;
  pe2d::Transform transform =
      pe2d::Transform(pe2d::Pos2d(pos.x, pos.y), pe2d::Angle());
  pe2d::Size2d size = pe2d::Size2d(50.0, 50.0);
  float staticFriction = 0.8;
  float dynamicFriction = 0.7;
  float restitution = 0.0;
  return pe2d::RigidBody(id, pe2d::Box, size, transform, mass, false,
                         pe2d::Vector2(0.0, 98.1), {}, 0.0, staticFriction,
                         dynamicFriction, restitution);
}

// returns circle with randomized parameters excluding frictions and restitution
pe2d::RigidBody GetRandomCircle(size_t id, sf::Vector2i pos) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribInt(30, 70);
  std::uniform_real_distribution<float> distribFloat(0.0, 1.0);
  std::uniform_int_distribution<int> distribPos(200, 800);
  std::uniform_int_distribution<int> distribRotation(0, 360);
  const pe2d::Transform transform =
      pe2d::Transform(pe2d::Pos2d(pos.x, pos.y), pe2d::Angle());
  const float mass = 1000.0;
  const float radius = 20.0;
  const float staticFriction = 1.0;
  const float dynamicFriction = 1.0;
  const float resistance = 0.0;
  return pe2d::RigidBody(id, pe2d::Circle, pe2d::Size2d(radius, radius),
                         transform, mass, false, pe2d::Vector2(0.0, 98.1), {},
                         0.0, staticFriction, dynamicFriction, resistance);
}

sf::Vector2f RotatePoint(const sf::Vector2f &point, const sf::Vector2f &center,
                         pe2d::Angle angle) {
  float radians = angle.AsRadians();
  float x = point.x - center.x;
  float y = point.y - center.y;

  float x_rotated = x * std::cos(radians) - y * std::sin(radians);
  float y_rotated = x * std::sin(radians) + y * std::cos(radians);

  return sf::Vector2f(x_rotated + center.x, y_rotated + center.y);
}

void DrawRigidBody(const pe2d::RigidBody body, const sf::Color &color,
                   sf::RenderWindow &window, bool draw_bounding_boxes = false) {
  if (body.GetType() == pe2d::Box) {
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
  } else if (body.GetType() == pe2d::Circle) {
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
    std::array<pe2d::Pos2d, 4> vertices = body.GetBoundingBox();
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

void DrawNormal(pe2d::Collision &collision, sf::RenderWindow &window) {
  const pe2d::CollisionPoints points = collision.GetCollisionPoints();
  const sf::Color color = sf::Color::Red;
  // Draw contact point
  sf::CircleShape contactPoint(2.0f);
  contactPoint.setFillColor(color);
  contactPoint.setPosition(sf::Vector2f(points.ContactPoint1.x - 2.0f,
                                        points.ContactPoint1.y - 2.0f));
  window.draw(contactPoint);

  // Draw normal
  sf::Vertex normal_line[] = {
      sf::Vertex(sf::Vector2f(points.ContactPoint1.x, points.ContactPoint1.y),
                 color),
      sf::Vertex(sf::Vector2f(points.ContactPoint1.x + points.Normal.x * 20.0f,
                              points.ContactPoint1.y + points.Normal.y * 20.0f),
                 color)};
  window.draw(normal_line, 2, sf::PrimitiveType::Lines);
}

void DrawTangent(pe2d::Collision &collision, sf::RenderWindow &window) {
  const pe2d::CollisionPoints points = collision.GetCollisionPoints();
  const pe2d::Vector2 rA_perp = pe2d::math::Perp(points.ContactPoint1);
  const pe2d::Vector2 rB_perp = pe2d::math::Perp(points.ContactPoint2);
  const sf::Color color = sf::Color::Green;

  const pe2d::Vector2 relative_velocity =
      (collision.GetObjectA().GetLinearVelocity() +
       (rA_perp * collision.GetObjectA().GetAngularVelocity())) -
      (collision.GetObjectB().GetLinearVelocity() +
       (rB_perp * collision.GetObjectB().GetAngularVelocity()));

  const pe2d::Vector2 tangent = pe2d::math::Normalize(
      relative_velocity -
      pe2d::math::Dot(relative_velocity, points.Normal) * points.Normal);
  sf::Vertex tangent_line[] = {
      sf::Vertex(sf::Vector2f(points.ContactPoint1.x, points.ContactPoint1.y),
                 color),
      sf::Vertex(sf::Vector2f(points.ContactPoint1.x + tangent.x * 20,
                              points.ContactPoint1.y + tangent.y * 20),
                 color)};
  window.draw(tangent_line, 2, sf::PrimitiveType::Lines);
}

void DrawImpulses(pe2d::Collision &collision, sf::RenderWindow &window) {
  const pe2d::RigidBody &rigid_bodyA = collision.GetObjectA();
  const pe2d::RigidBody &rigid_bodyB = collision.GetObjectB();

  const pe2d::CollisionPoints &points = collision.GetCollisionPoints();
  const double inv_massA = rigid_bodyA.GetInvMass();
  const double inv_massB = rigid_bodyB.GetInvMass();
  const double inv_inertiaA = rigid_bodyA.GetInvRotationalInertia();
  const double inv_inertiaB = rigid_bodyB.GetInvRotationalInertia();
  const pe2d::Vector2 normal = points.Normal;
  const unsigned int contact_count = points.ContactCount;
  const double restitution_coefficient =
      (rigid_bodyA.GetRestitution() + rigid_bodyB.GetRestitution()) * 0.5;
  const std::array<pe2d::Pos2d, 2> contact_list = {points.ContactPoint1,
                                                   points.ContactPoint2};

  pe2d::Vector2 minimal_translation_vector = normal * points.Depth;
  if (pe2d::math::Dot(minimal_translation_vector,
                      rigid_bodyA.GetPosition() - rigid_bodyB.GetPosition()) <
      0.0f) {
    minimal_translation_vector *= -1.0f;
  }

  // list of vectors pointing from object A's center of mass to the contact
  // points
  std::array<pe2d::Vec2d, 2> rA_list;
  // list of vectors pointing from object A's center of mass to the contact
  // points
  std::array<pe2d::Vec2d, 2> rB_list;
  // collision impulses along normal
  std::array<pe2d::Vec2d, 2> impulses;
  // friction impulses along normal
  std::array<pe2d::Vec2d, 2> friction_impulses;

  std::array<double, 2> impulse_scalars;

  // calculate collision impulses
  for (std::size_t i = 0; i < contact_count; i++) {
    rA_list[i] = contact_list[i] - rigid_bodyA.GetPosition();
    rB_list[i] = contact_list[i] - rigid_bodyB.GetPosition();

    const pe2d::Vector2 rA_perp = pe2d::math::Perp(rA_list[i]);
    const pe2d::Vector2 rB_perp = pe2d::math::Perp(rB_list[i]);

    const pe2d::Vector2 relative_velocity =
        (rigid_bodyA.GetLinearVelocity() +
         (rA_perp * rigid_bodyA.GetAngularVelocity())) -
        (rigid_bodyB.GetLinearVelocity() +
         (rB_perp * rigid_bodyB.GetAngularVelocity()));

    const double rA_perp_normal = pe2d::math::Dot(rA_perp, normal);
    const double rB_perp_normal = pe2d::math::Dot(rB_perp, normal);

    const double denominator =
        inv_massA + inv_massB +
        (rA_perp_normal * rA_perp_normal) * inv_inertiaA +
        (rB_perp_normal * rB_perp_normal) * inv_inertiaB;

    double impulse_scalar = -(1.0 + restitution_coefficient) *
               pe2d::math::Dot(relative_velocity, normal);
    impulse_scalar /= denominator;
    impulse_scalar /= (double)contact_count;
    impulse_scalars[i] = impulse_scalar;

    impulses[i] = impulse_scalar * normal;
  }

  const double static_friction_coefficient =
      (rigid_bodyA.GetStaticFriction() + rigid_bodyB.GetStaticFriction()) * 0.5;

  const double dynamic_friction_coefficient =
      (rigid_bodyA.GetDynamicFriction() + rigid_bodyB.GetDynamicFriction()) *
      0.5;

  // calculate friction impulses
  for (std::size_t i = 0; i < contact_count; i++) {
    rA_list[i] = contact_list[i] - rigid_bodyA.GetPosition();
    rB_list[i] = contact_list[i] - rigid_bodyB.GetPosition();

    const pe2d::Vector2 rA_perp = pe2d::math::Perp(rA_list[i]);
    const pe2d::Vector2 rB_perp = pe2d::math::Perp(rB_list[i]);

    const pe2d::Vector2 relative_velocity =
        (rigid_bodyA.GetLinearVelocity() +
         (rA_perp * rigid_bodyA.GetAngularVelocity())) -
        (rigid_bodyB.GetLinearVelocity() +
         (rB_perp * rigid_bodyB.GetAngularVelocity()));

    pe2d::Vector2 tangent =
        relative_velocity * pe2d::math::Dot(relative_velocity, normal) * normal;
    if (pe2d::math::NearlyEquel(tangent, pe2d::Vec2d(), 0.00000025)) {
      continue;
    } else {
      tangent = pe2d::math::Normalize(tangent);
    }

    const float rA_perp_tangent = pe2d::math::Dot(rA_perp, tangent);
    const float rB_perp_tangent = pe2d::math::Dot(rB_perp, tangent);

    const double denominator =
        inv_massA + inv_massB +
        (rA_perp_tangent * rA_perp_tangent) * inv_inertiaA +
        (rB_perp_tangent * rB_perp_tangent) * inv_inertiaB;

    float friction_impulse_scalar = -pe2d::math::Dot(relative_velocity, tangent);
    friction_impulse_scalar /= denominator;
    friction_impulse_scalar /= (double)contact_count;

    const float impulse_scalar = impulse_scalars[i];
    if (std::abs(friction_impulse_scalar) <= impulse_scalar * static_friction_coefficient) {
      friction_impulses[i] = friction_impulse_scalar * tangent;
    } else {
      friction_impulses[i] = impulse_scalar * tangent * dynamic_friction_coefficient;
    }
  }

  auto draw_impulse = [&window](pe2d::Pos2d point, pe2d::Vec2d impulse,
                                sf::Color color) {
    sf::Vertex impulse_line[] = {
        sf::Vertex(sf::Vector2f(point.x, point.y), color),
        sf::Vertex(sf::Vector2f(point.x + impulse.x, point.y + impulse.y),
                   color)};
    window.draw(impulse_line, 2, sf::PrimitiveType::Lines);
  };
  draw_impulse(points.ContactPoint1, impulses[0], sf::Color::Yellow);
  draw_impulse(points.ContactPoint1, impulses[1], sf::Color::Yellow);
  draw_impulse(points.ContactPoint2, friction_impulses[0], sf::Color::Magenta);
  draw_impulse(points.ContactPoint2, friction_impulses[1], sf::Color::Magenta);
}

VisualWorld::VisualWorld(unsigned int substeps) : m_PhysicsWorld(substeps) {
  m_PhysicsWorld.AddGrid({0, 0},
    pe2d::Size2i(1000, 1000), 100);
  m_PhysicsWorld.SetSolver(pe2d::ImpulseSolver);
}

void VisualWorld::SetUp() {
  m_PhysicsWorld.AddObject(pe2d::RigidBody(
      1, pe2d::Box, pe2d::Size2d(950, 100), pe2d::Transform({500.0, 850.0}),
      0.0, true, {}, pe2d::Vec2d(), 0.0, 0.8, 1.0, 0.0));

  m_PhysicsWorld.AddObject(pe2d::RigidBody(
      2, pe2d::Box, pe2d::Size2d(300.0, 50.0),
      pe2d::Transform({700.0, 650.0}, pe2d::Angle::FromDegrees(90.0)), 0.0,
      true, {}, pe2d::Vec2d(), 0.0, 0.8, 1.0, 0.0));

  m_PhysicsWorld.AddObject(pe2d::RigidBody(
      3, pe2d::Box, pe2d::Size2d(300, 50),
      pe2d::Transform({950.0, 650.0}, pe2d::Angle::FromDegrees(90.0)), 0.0,
      true, {}, pe2d::Vec2d(), 0.0, 0.8, 1.0, 0.0));

  m_PhysicsWorld.AddObject(pe2d::RigidBody(
      4, pe2d::Box, pe2d::Size2d(350, 50),
      pe2d::Transform({250.0, 450.0}, pe2d::Angle::FromDegrees(45.0)), 0.0,
      true, {}, pe2d::Vec2d(), 0.0, 0.8, 1.0, 0.0));
  
  m_LastId = 4;
  ASSERT(m_LastId == m_PysicsWorld.At(4).GetIndex(),
         "m_LastId have to match the id of last rigid body in m_PhysicsWorld");
}

void VisualWorld::Update(sf::Vector2i position, float delta_time) {
  m_LastSpawnTime += delta_time;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
      m_LastSpawnTime >= SPAWN_COOLDOWN) {
    m_LastId += 1;
    const pe2d::RigidBody box = GetRandomBox(m_LastId, position);
    m_PhysicsWorld.AddObject(box);
    m_LastSpawnTime = 0.0f;
  } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) &&
             m_LastSpawnTime >= SPAWN_COOLDOWN) {
    m_LastId += 1;
    const pe2d::RigidBody circle = GetRandomCircle(m_LastId, position);
    m_PhysicsWorld.AddObject(circle);
    m_LastSpawnTime = 0.0f;
  }
}

void VisualWorld::Draw(sf::RenderWindow &window, const SceneSettings &scene_settings) {
  for (auto it = m_PhysicsWorld.cBegin(); it != m_PhysicsWorld.cEnd(); it++) {
    sf::Color color;
    if (it->second.GetType() == pe2d::Box) {
      DrawRigidBody(it->second, sf::Color::Blue, window, false);
    } else {
      DrawRigidBody(it->second, sf::Color::Red, window, false);
    }
  }
  if(scene_settings.DrawImpulses) {
    for(auto &collision : m_Collisions ) {
      DrawImpulses(collision, window);
    }
  } 
  if(scene_settings.DrawNormalLines) {
    for(auto &collision : m_Collisions ) {
      DrawNormal(collision, window);
    }
  }
  if(scene_settings.DrawTangentLines) {
    for(auto &collision : m_Collisions ) {
      DrawTangent(collision, window);
    }
  }
}

void VisualWorld::Step(float delta_time) { 
  m_Collisions.clear();
  m_PhysicsWorld.Step(delta_time);
  std::list<std::pair<size_t, size_t>> pairs =
      m_PhysicsWorld.m_Grid.GetCollisionPairs();
  for (auto it = pairs.begin(); it != pairs.end(); it++) {
    m_PhysicsWorld.FindCollisions(it->first, it->second, m_Collisions);
  } 
}
