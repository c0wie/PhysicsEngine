#include "FallingCirclesTest.hpp"
#include "../../pe2d/include/Algo.hpp"
namespace test
{

    FallingCirclesTest::FallingCirclesTest()
    {
        std::shared_ptr<pe2d::Solver> solver = std::make_shared<pe2d::PositionSolver>();
        m_World.AddSolver(solver);
        m_World.AddGrid(pe2d::Vector2(-200.0f, -200.0f), pe2d::Vector2(1200.0f, 1200.0f), 200.0f);
    }

    void FallingCirclesTest::OnUpdate(float deltaTime, sf::Vector2i mousePos)
    {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            pe2d::Vector2 position = pe2d::Vector2{(float)mousePos.x, (float)mousePos.y};
            pe2d::Transform transform = pe2d::Transform{position, pe2d::Vector2(1.0f, 1.0f), 0.0f};
            AddCircle(m_World.Size(), sf::Color::Magenta, 40.0f, transform, false, true, 10.0f, pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2{0.0f, 10.0f});
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