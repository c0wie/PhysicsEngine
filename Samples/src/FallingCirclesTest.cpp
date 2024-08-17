#include "FallingCirclesTest.hpp"
#include "../../pe2d/include/Algo.hpp"
namespace test
{

    FallingCirclesTest::FallingCirclesTest() :
        m_TimeSinceLastSpawn(0.0f)
    {
        std::shared_ptr<pe2d::Solver> solver = std::make_shared<pe2d::ImpulseSolver>();
        m_World.AddSolver(solver);
        m_World.AddGrid(pe2d::Vector2(-200.0f, -200.0f), pe2d::Vector2(1200.0f, 1200.0f), 200.0f);
    }

    void FallingCirclesTest::OnUpdate(float deltaTime, sf::Vector2i mousePos)
    {
        m_TimeSinceLastSpawn += deltaTime;
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && m_TimeSinceLastSpawn >= m_SquareSpawnCooldown)
        {
            m_TimeSinceLastSpawn = 0.0f;
            const pe2d::Vector2 position = pe2d::Vector2((float)mousePos.x, (float)mousePos.y);
            const pe2d::Transform transform = pe2d::Transform{position, pe2d::Vector2(1.0f, 1.0f), 0.0f};
            constexpr float mass = 10.0f;
            constexpr float radius = 40.0f;
            constexpr pe2d::Vector2 velocity = pe2d::Vector2(0.0f, 0.0f);
            constexpr pe2d::Vector2 gravity = pe2d::Vector2(0.0f, 98.1f);
            constexpr float staticFriction = 0.5f;
            constexpr float dynamicFriction = 0.5f;
            AddCircle(m_World.Size(), sf::Color::Magenta, radius, transform, false, mass,
                    velocity, gravity, staticFriction, dynamicFriction, 0.0f);
        }
        m_World.Step(deltaTime);  
    }

    void FallingCirclesTest::OnRender(sf::RenderWindow &window)
    {
        Draw(window);
    }

    void FallingCirclesTest::OnImGuiRender(sf::RenderWindow &window, const ImGuiIO &io)
    {
        if( ImGui::Button("Clear Objects") )
        {
            m_World.ClearObjects();
            m_Shapes.clear();
        }
        ImGui::Text("Number of objects: %i", m_World.Size());
        ImGui::Text("Application average %i ms/frame (%i FPS)", (int)(1000.0f / io.Framerate), (int)io.Framerate);
    }
}   