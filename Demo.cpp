#include <chrono>

#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include "PhysicsWorld.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(1000.f, 1000.f), "DEMO");
    ImGui::SFML::Init(window);
    phy::DynamicsWorld physicsWorld;

    
    std::shared_ptr<phy::PositionSolver> positionSolver = std::make_shared<phy::PositionSolver>();
    physicsWorld.AddSolver(positionSolver);

    {
        phy::Vector2 position = phy::Vector2{500.0f, 500.f};
        std::shared_ptr<phy::Transform> transform = std::make_shared<phy::Transform> (position, phy::Vector2{4.25f, 1.0f}, 0.0f);
        std::shared_ptr<phy::SquareCollider> collider = std::make_shared<phy::SquareCollider>(200.0f);

        std::shared_ptr<phy::CollisionObject> obj = std::make_shared<phy::CollisionObject>(collider, transform, false);
        physicsWorld.AddCollisionObject(obj);
    }

    {
        phy::Vector2 position = phy::Vector2{520, 140.f};
        std::shared_ptr<phy::Transform> transform = std::make_shared<phy::Transform> (position, phy::Vector2{1.0f, 1.0f}, 0.0f);
        std::shared_ptr<phy::SquareCollider> collider = std::make_shared<phy::SquareCollider>(75.0f);
        
        float mass = 10.0f;
        phy::Vector2 velocity = phy::Vector2{0.0f, 0.0f};
        phy::Vector2 force = phy::Vector2{0.0f, 0.0f};
        phy::Vector2 gravity = {0.0f, 9.81f};

        std::shared_ptr<phy::RigidObject> obj = 
            std::make_shared<phy::RigidObject>(collider, transform, false, mass, velocity, force, gravity, false, 0.0f, 0.0f, 0.0f);
        physicsWorld.AddCollisionObject(obj);
    }

    {
        phy::Vector2 position = phy::Vector2{250.0f, 140.f};
        std::shared_ptr<phy::Transform> transform = std::make_shared<phy::Transform> (position, phy::Vector2{1.0f, 1.0f}, 0.0f);
        std::shared_ptr<phy::CircleCollider> collider = std::make_shared<phy::CircleCollider>(25.0f);

        float mass = 50.0f;
        phy::Vector2 velocity = phy::Vector2{0.0f, 0.0f};
        phy::Vector2 force = phy::Vector2{0.0f, 0.0f};
        phy::Vector2 gravity = {0.0f, 9.81f};

        std::shared_ptr<phy::RigidObject> obj = 
            std::make_shared<phy::RigidObject>(collider, transform, false, mass, velocity, force, gravity, false, 0.0f, 0.0f, 0.0f);
        physicsWorld.AddCollisionObject(obj);
    }
    
    {
        phy::Vector2 position = phy::Vector2{750.0f, 250.f};
        std::shared_ptr<phy::Transform> transform = std::make_shared<phy::Transform> (position, phy::Vector2{1.0f, 1.0f}, 0.0f);
        std::shared_ptr<phy::SquareCollider> collider = std::make_shared<phy::SquareCollider>(50.0f);

        std::shared_ptr<phy::CollisionObject> obj = std::make_shared<phy::CollisionObject>(collider, transform, false);
        physicsWorld.AddCollisionObject(obj);
    }

    sf::Clock DT_Clock;
    float deltaTime = 0.0f;
    DT_Clock.restart();
    
    while(window.isOpen())
    {
        deltaTime = DT_Clock.restart().asSeconds();
        sf::Event event;
        while(window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        ImGui::SFML::Update(window, DT_Clock.restart());
        
        window.clear();
        ImGui::SFML::Render(window);
        physicsWorld.Step(deltaTime);
        window.display();
    }
    ImGui::SFML::Shutdown();
}