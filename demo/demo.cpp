// local
#include "scene_settings.hpp"
#include "solver.hpp"
#include "vector2.hpp"
#include "visual_world.hpp"

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
  VisualWorld visual_world(8);
  visual_world.SetUp();

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
    visual_world.Update(sf::Mouse::getPosition(window), delta_time.asSeconds());
    visual_world.Step(delta_time.asSeconds());
    
    ImGui::SFML::Update(window, delta_time);
    
    if(ImGui::Begin("Scene settings")) {
      if (scene_settings.IsGridOn) {
        if (ImGui::Button("Grid: ON")) {
          scene_settings.IsGridOn = false;
          visual_world.GetPhysicsWorld().RemoveGrid();
        }
      } else {
        if (ImGui::Button("Grid: OFF")) {
          scene_settings.IsGridOn = true;
          visual_world.GetPhysicsWorld().AddGrid({-100, -100},
                                                 pe2d::Size2i(1000, 1000), 50);
        }
      }
      if (scene_settings.DrawImpulses) {
        if (ImGui::Button("Draw Impulses: ON")) {
          scene_settings.DrawImpulses = false;
        }
      } else {
        if (ImGui::Button("Draw Impulses: OFF")) {
          scene_settings.DrawImpulses = true;
        }
      }
      if (scene_settings.DrawNormalLines) {
        if (ImGui::Button("Draw Collision Normals: ON")) {
          scene_settings.DrawNormalLines = false;
        }
      } else {
        if (ImGui::Button("Draw Collision Normals: OFF")) {
          scene_settings.DrawNormalLines = true;
        }
      }
      if (scene_settings.DrawTangentLines) {
        if (ImGui::Button("Draw Collision Tangent: ON")) {
          scene_settings.DrawTangentLines = false;
        }
      } else {
        if (ImGui::Button("Draw Collision Tangent: OFF")) {
          scene_settings.DrawTangentLines = true;
        }
      }
      if(scene_settings.Solver == SolverType::POSITION_SOLVER) {
        if(ImGui::Button("Solver type: POSITION_SOLVER")) {
          scene_settings.Solver = SolverType::IMPULSE_SOLVER;
          visual_world.GetPhysicsWorld().SetSolver(pe2d::ImpulseSolver);
        }
      } else if(scene_settings.Solver == SolverType::IMPULSE_SOLVER) {
        if(ImGui::Button("Solver type: IMPULSE_SOLVER")) {
          scene_settings.Solver = SolverType::POSITION_SOLVER;
          visual_world.GetPhysicsWorld().SetSolver(pe2d::PositionSolver);
        }
      }
      if(ImGui::Button("Clear objects")) {
        visual_world.GetPhysicsWorld().ClearObjects();
        visual_world.SetUp();
      }
      ImGui::Text("Number of rigid bodies: %zu", visual_world.GetPhysicsWorld().Size());
      ImGui::Text("FPS %i", (int)ImGui::GetIO().Framerate);
    }
    ImGui::End();

    window.clear();
    visual_world.Draw(window, scene_settings);
    ImGui::SFML::Render(window);
    window.display();
  }
  ImGui::SFML::Shutdown();
}