#include <chrono>

#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include "PhysicsWorld.hpp"
#include "tests/include/TestCollision.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(1000.f, 1000.f), "DEMO", sf::Style::Titlebar | sf::Style::Close);
    ImGui::SFML::Init(window);

    test::TestCollision test;

    sf::Clock DT_Clock;
    float deltaTime = 0.0f;
    DT_Clock.restart();
    
    while(window.isOpen())
    {
        deltaTime = DT_Clock.restart().asSeconds();

        if (deltaTime <= 0.0f) 
        {
            deltaTime = 1.0f / 60.0f; // Fallback to a default frame rate
        }   
        sf::Event event;
        while(window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        sf::Time clock = sf::seconds(deltaTime);
        ImGui::SFML::Update(window, clock);
        test.OnUpdate(deltaTime);

        window.clear();
        test.OnRender(window);
        test.OnImGuiRender(window);
        ImGui::SFML::Render(window);
        
        window.display();
    }
    ImGui::SFML::Shutdown();
}