#include "TestCollision.hpp"
#include "../../pe2d/include/Algo.hpp"
namespace test
{

    TestCollision::TestCollision() :
        Test(true),
        showObjectEditor(false)
    {
        m_World.SetPartitioningSystem(pe2d::Vector2{-100.0f, -100.0f}, pe2d::Vector2{1100.0f, 1100.0f}, 5U);
        std::shared_ptr<pe2d::Solver> solver = std::make_shared<pe2d::PositionSolver>();
        m_World.AddSolver(solver);
        ResetVariables();        
    }

    void TestCollision::OnUpdate(float deltaTime, sf::Vector2i mousePos)
    {
        const auto objects = m_World.GetObjects();
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            pe2d::Vector2 position = pe2d::Vector2{(float)mousePos.x, (float)mousePos.y};
            pe2d::Transform transform = pe2d::Transform{position, pe2d::Vector2(1.0f, 1.0f), 0.0f};
            AddCircle(m_World.GetObjectsCount(), sf::Color::Magenta, 40.0f, transform, false, 10.0f, pe2d::Vector2{}, pe2d::Vector2{0.0f, 10.0f});
            m_World.Step(deltaTime);  
        }
        m_World.Step(deltaTime);  
    }

    void TestCollision::OnRender(sf::RenderWindow &window)
    {
        Draw(window);
    }

    void TestCollision::OnImGuiRender(sf::RenderWindow &window, const ImGuiIO &io)
    {
        if( ImGui::Button("Add Object") )
        {
            showObjectEditor = true;
        }
        if( ImGui::Button("Clear Objects") )
        {
            m_World.ClearObjects();
            m_Shapes.clear();
        }
        ImGui::Text("Number of objects: %i", m_World.GetObjects().size());
        ImGui::Text("Application average %i ms/frame (%i FPS)", (int)(1000.0f / io.Framerate), (int)io.Framerate);
        if(showObjectEditor)
        {
            ImGui::Begin("Object Editor");
            CollisionObjectInput();
            if( ImGui::Button("CollisionObject") )
            {
                isRigidObject = false;
            }
            ImGui::SameLine();
            if( ImGui::Button("RigidObject") )
            {
                isRigidObject = true;
            }
            if(isRigidObject)
            {
                RigidObjectInput();                
            }
            if( ImGui::Button("Reset Variables") )
            {
                ResetVariables();
            }
            if( ImGui::Button("Create") )
            {
                CreateObject();
            }
            ImGui::End();
        }
    }

    void TestCollision::CollisionObjectInput()
    {
        if(ID == ID::BOX)
        {
            if( ImGui::Button("Make Circle") )
            {
                ID = ID::CIRCLE;
            }
            ImGui::InputFloat2("Size", &size.x);
        }
        else if(ID == ID::CIRCLE)
        {
            if( ImGui::Button("Make Square") )
            {
                ID = ID::BOX;
            }
            ImGui::InputFloat("Radius", &radius);
        }
        ImGui::InputFloat2("Position", &position.x);
        ImGui::InputFloat2("Scale", &scale.x);
        ImGui::InputFloat("Rotation", &rotation);
        ImGui::InputInt3("Color", &color.red);
    }
   
    void TestCollision::RigidObjectInput()
    {
        ImGui::InputFloat("Mass", &mass);
        ImGui::InputFloat2("Velocity", &velocity.x);
        ImGui::InputFloat2("Gravity", &gravity.x);
    } 

    void TestCollision::ResetVariables()
    {
        isRigidObject = false;
        ID = ID::BOX;
        size = pe2d::Vector2{};
        radius = 0.0f;
        position = pe2d::Vector2{500.0f, 500.0f};
        scale = pe2d::Vector2{1.0f, 1.0f};
        rotation = 0.0f;
        color = {255, 255, 255};
        mass = 0.0f;
        velocity = pe2d::Vector2{};
        gravity = pe2d::Vector2{};
    }

    void TestCollision::CreateObject()
    {
        const sf::Color Color = sf::Color{(sf::Uint8)color.red, (sf::Uint8)color.green, (sf::Uint8)color.blue};
        const pe2d::Transform transform = pe2d::Transform{ position, scale, rotation};
        const unsigned int ID = m_World.GetObjectsCount();
        if(ID == ID::BOX)
        {
            if(isRigidObject)
            {
                AddBox(ID, Color, size, transform, false, mass, velocity, gravity);
            }
            else
            {
                AddBox(ID, Color, size, transform, false);
            }
        }
        else if(ID == ID::CIRCLE)
        {
            if(isRigidObject)
            {
                AddCircle(ID, Color, radius, transform, false, mass, velocity, gravity);
            }
            else
            {
                AddCircle(ID, Color, radius, transform, false);
            }
        }
        showObjectEditor = false;
    }
}   