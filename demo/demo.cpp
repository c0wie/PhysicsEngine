// local
#include "game_world.hpp"
#include "rigid_body.hpp"

// lib
//SFML
#include <SFML/System/Time.hpp>
// imgui
#include <imgui-SFML.h>
#include <imgui.h>

// std
#include <cstdlib>
#include <iostream>

int main() {
  sf::RenderWindow window(sf::VideoMode({1000, 1000}), "DEMO",
                          sf::Style::Titlebar | sf::Style::Close);
  if (!ImGui::SFML::Init(window)) {
    std::cerr << "ImGui::SFML initialiaztion failed\n";
    std::exit(EXIT_FAILURE);
  }

  SceneSettings scene_settings;
  GameWorld game_world(8);

  float delta_time = 0.0f;
  bool draw_bounding_boxes = false;

  sf::Clock DT_Clock;
  while (window.isOpen()) {
    sf::Time delta_time = DT_Clock.restart();

    while (const std::optional event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }
    game_world.Update(sf::Mouse::getPosition(window), delta_time.asSeconds());

    ImGui::SFML::Update(window, delta_time);

    if(ImGui::Begin("Scene settings")) {
      if (scene_settings.IsGridOn) {
        if (ImGui::Button("Grid: ON")) {
          scene_settings.IsGridOn = false;
          game_world.RemoveGrid();
        }
      } else {
        if (ImGui::Button("Grid: OFF")) {
          scene_settings.IsGridOn = true;
          game_world.AddGrid({-100, -100}, 100, 100, 100);
        }
      }
      if(scene_settings.Solver == SolverType::POSITION_SOLVER) {
        if(ImGui::Button("Solver type: POSITION_SOLVER")) {
          scene_settings.Solver = SolverType::IMPULSE_SOLVER;
          game_world.SetSolver(pe2d::ImpulseSolver);
        }
      } else if(scene_settings.Solver == SolverType::IMPULSE_SOLVER) {
        if(ImGui::Button("Solver type: IMPULSE_SOLVER")) {
          scene_settings.Solver = SolverType::POSITION_SOLVER;
          game_world.SetSolver(pe2d::PositionSolver);
        }
      }
      if(ImGui::Button("Clear objects")) {
        game_world.Clear();
      }
      if (ImGui::CollapsingHeader("Last object information")) {
        const Entity entity = game_world.GetLastEntity();
        const pe2d::RigidBody &rigid_object = entity.core;
        ImGui::Text("ID: %zu", game_world.Size() - 1);
        if (rigid_object.IsStatic()) {
          ImGui::Text("Static object");
        } else {
          ImGui::Text("Non static object");
        }
        ImGui::Text("Position: %f, %f", rigid_object.GetPosition().x,
                    rigid_object.GetPosition().y);
        ImGui::Text("Rotation(degrees): %f",
                    rigid_object.GetAngle().AsDegrees());
        ImGui::Text("Mass: %f", rigid_object.GetMass());
        ImGui::Text("Rotational inertia: %f",
                    rigid_object.GetRotationalInertia());
        ImGui::Text("Angular velocity: %f", rigid_object.GetAngularVelocity());
        ImGui::Text("Linear velocity: %f, %f",
                    rigid_object.GetLinearVelocity().x,
                    rigid_object.GetLinearVelocity().y);
        ImGui::Text("Static friction: %f", rigid_object.GetStaticFriction());
        ImGui::Text("Dynamic friction %f", rigid_object.GetDynamicFriction());
        ImGui::Text("Restitution %f", rigid_object.GetRestitution());
        ImGui::Text("Force: %f, %f", rigid_object.GetForce().x,
                    rigid_object.GetForce().y);
      }
      ImGui::Text("Number of rigid bodies: %zu", game_world.Size());
      ImGui::Text("FPS %i", (int)ImGui::GetIO().Framerate);
    }
    ImGui::End();

    window.clear();
    game_world.Draw(window, scene_settings);
    ImGui::SFML::Render(window);
    window.display();
  }
  ImGui::SFML::Shutdown();
}