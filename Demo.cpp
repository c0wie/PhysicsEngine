#include <chrono>

#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include "PhysicsWorld.hpp"
#include "Algo.hpp"
#include "Sample/include/CollisionArenaTest.hpp"
#include "Sample/include/TestMenu.hpp"


int main()
{
    sf::RenderWindow window(sf::VideoMode(1000.f, 1000.f), "DEMO", sf::Style::Titlebar | sf::Style::Close);
    ImGui::SFML::Init(window);

    test::Test* currentTest = nullptr;
    test::TestMenu *testMenu = new test::TestMenu(currentTest, pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(1000.0f, 1000.0f), 1U);
    currentTest = testMenu;

    testMenu->RegisterTest<test::CollisionArenaTest>("Collision Arena Test");

    sf::Vector2f vec;
    sf::Clock DT_Clock;
    float deltaTime = 0.0f;
    DT_Clock.restart();
    ImGuiIO& io = ImGui::GetIO();

    while(window.isOpen())
    {
        deltaTime = DT_Clock.restart().asSeconds();
        sf::Time clock = sf::seconds(deltaTime);

        sf::Event event;
        while(window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, clock);
        window.clear();
        if (currentTest)
        {
            currentTest->OnUpdate(deltaTime, sf::Mouse::getPosition(window));
            currentTest->OnRender(window); 
            ImGui::Begin("TEST");
            if (currentTest != testMenu && ImGui::Button("<-"))
            {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->OnImGuiRender(window, io);

        }            
        ImGui::End();
        ImGui::SFML::Render(window);
        window.display();
    }
    delete testMenu;
    ImGui::SFML::Shutdown();
}