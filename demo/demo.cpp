// pe2d
#include "algo.hpp"
#include "math.hpp"
#include "physics_world.hpp"
#include "vector2.hpp"

// sfml
#include <SFML/Graphics.hpp>

// imgui
#include <SFML/Window/Event.hpp>
#include <array>
#include <imgui-SFML.h>
#include <imgui.h>

// std
#include <cstdlib>
#include <iostream>
#include <random>

#define DEBUG 0
// returns box with randomized parameters excluding frictions and restitution
pe2d::RigidBody GetRandomBox(size_t id, sf::Vector2i pos) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribInt(30, 150);
  std::uniform_real_distribution<float> distribFloat(0.0, 1.0);
  std::uniform_int_distribution<int> distribPos(200, 800);
  std::uniform_int_distribution<int> distribRotation(0, 180);
  float mass = 100.f;
  pe2d::Transform transform = pe2d::Transform(pe2d::Pos2d(pos.x, pos.y), 0.0);
  pe2d::Size2d size = pe2d::Size2d(100.0, 100.0);
  float staticFriction = 1.0;
  float dynamicFriction = 1.0;
  float restitution = 0.0;
  return pe2d::RigidBody(id, pe2d::Box, size, transform, mass, false,
                         pe2d::Vector2(0.0, 98.1), {}, 0.0);
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
      pe2d::Transform(pe2d::Pos2d(pos.x, pos.y), 0.0);
  const float mass = 1.0;
  const float radius = 50.0;
  const float staticFriction = 1.0;
  const float dynamicFriction = 1.0;
  const float resistance = 0.0;
  return pe2d::RigidBody(id, pe2d::Circle, pe2d::Size2d(radius, radius),
                         transform, mass, false, pe2d::Vector2(0.0, 98.1), {},
                         0.0, staticFriction, dynamicFriction, resistance);
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
    rec.setRotation(sf::Angle(sf::radians(body.GetRotation())));
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
    line[1].position = sf::Vector2f(position.x + radius, position.y);
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

int main() {
#if DEBUG == 0
  sf::RenderWindow window(sf::VideoMode({1000, 1000}), "DEMO",
                          sf::Style::Titlebar | sf::Style::Close);
  if (!ImGui::SFML::Init(window)) {
    std::cerr << "ImGui::SFML initialiaztion failed\n";
    std::exit(EXIT_FAILURE);
  }

  pe2d::PhysicsWorld physics_world(8);
  physics_world.AddGrid(pe2d::Pos2d(0.0, 0.0), pe2d::Size2i(10, 10), 100.0f);
  physics_world.SetSolver(pe2d::ImpulseSolverWithoutFriction);
  physics_world.AddObject(pe2d::RigidBody(1, pe2d::Box, pe2d::Size2d(800, 100),
                                          pe2d::Transform({500.0, 850.0}), 0.0,
                                          true, {}));
  physics_world.AddObject(pe2d::RigidBody(
      2, pe2d::Box, pe2d::Size2d(300.0, 50.0),
      pe2d::Transform({300.0, 400.0}, pe2d::math::DeegresToRadians(45.0)), 0.0,
      true, {}));
  physics_world.AddObject(pe2d::RigidBody(
      3, pe2d::Box, pe2d::Size2d(300, 50),
      pe2d::Transform({700.0, 500.0}, pe2d::math::DeegresToRadians(75)), 0.0,
      true, {}));

  sf::Clock DT_Clock;
  const float spawn_cooldown = 0.1f;
  float deltaTime = 0.0f;
  float time_since_last_spawn;
  ImGuiIO &io = ImGui::GetIO();
  bool draw_bounding_boxes = false;
  std::size_t last_id = 3;

  DT_Clock.restart();
  while (window.isOpen()) {
    deltaTime = DT_Clock.restart().asSeconds();
    sf::Time clock = sf::seconds(deltaTime);

    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    ImGui::SFML::Update(window, clock);
    window.clear();
    time_since_last_spawn += deltaTime;
    const auto position = sf::Mouse::getPosition(window);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
        time_since_last_spawn >= spawn_cooldown) {
      last_id += 1;
      const pe2d::RigidBody box = GetRandomBox(last_id, position);
      physics_world.AddObject(box);
      time_since_last_spawn = 0.0f;
    } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) &&
               time_since_last_spawn >= spawn_cooldown) {
      last_id += 1;
      const pe2d::RigidBody circle = GetRandomCircle(last_id, position);
      physics_world.AddObject(circle);
      time_since_last_spawn = 0.0f;
    }
    physics_world.Step(deltaTime);

    for (auto it = physics_world.cBegin(); it != physics_world.cEnd(); it++) {
      sf::Color color;
      if (it->second.GetType() == pe2d::Box) {
        DrawRigidBody(it->second, sf::Color::Blue, window, draw_bounding_boxes);
      } else {
        DrawRigidBody(it->second, sf::Color::Red, window, draw_bounding_boxes);
      }
    }
    ImGui::Begin("TEST");
    const pe2d::Vec2d linear_velocityA =
        physics_world.At(last_id).GetLinearVelocity();
    const float angular_velocityA =
        physics_world.At(last_id).GetAngularVelocity();
    const pe2d::Vec2d linear_velocityB =
        physics_world.At(last_id - 1).GetLinearVelocity();
    const float angular_velocityB =
        physics_world.At(last_id - 1).GetAngularVelocity();
    ImGui::Text("Linear velocityA: %i, %i", (int)(linear_velocityA.x),
                (int)(linear_velocityA.y));
    ImGui::Text("Angular velocityA: %i", (int)(angular_velocityA));
    ImGui::Text("Linear velocityB: %i, %i", (int)(linear_velocityB.x),
                (int)(linear_velocityB.y));
    ImGui::Text("Angular velocityB: %i", (int)(angular_velocityB));
    ImGui::Text("Number of objects: %zu", physics_world.Size());
    ImGui::End();
    ImGui::SFML::Render(window);
    window.display();
  }
  ImGui::SFML::Shutdown();
#endif

  auto print_arr = [](const std::array<pe2d::Pos2d, 4> &arr) {
    for (const pe2d::Pos2d elem : arr) {
      std::cout << " " << elem.GetString();
    }
    std::cout << '\n';
  };

  pe2d::Pos2d m_Center = pe2d::Pos2d(500.0, 500.0);
  std::array<pe2d::Pos2d, 4> m_TestVertices = {
      pe2d::Pos2d(450.0, 450.0), pe2d::Pos2d(550.0, 450.0),
      pe2d::Pos2d(550.0, 550.0), pe2d::Pos2d(450.0, 550.0)};
  auto ver45 = m_TestVertices;
  pe2d::algo::RotateVertices(ver45, m_Center,
                             pe2d::math::DeegresToRadians(45.0));
  std::cout << "45 deegres\n";
  print_arr(ver45);

  auto ver90 = m_TestVertices;
  pe2d::algo::RotateVertices(ver45, m_Center,
                             pe2d::math::DeegresToRadians(90.0));
  std::cout << "90 deegres\n";
  print_arr(ver90);

  auto ver189 = m_TestVertices;
  pe2d::algo::RotateVertices(ver189, m_Center,
                             pe2d::math::DeegresToRadians(189.0));
  std::cout << "189 deegres\n";
  print_arr(ver189);
}