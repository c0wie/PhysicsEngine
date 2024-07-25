#include "CollisionArenaTest.hpp"

namespace test
{
    CollisionArenaTest::CollisionArenaTest() :
        showObjectEditor(false),
        showPartitioningSystemEditor(false)
    {
        m_World.SetPartitioningSystem(pe2d::Vector2(-100.0f, -100.0f), pe2d::Vector2(1100.0f, 1100.0f), 5U);
        std::shared_ptr<pe2d::Solver> solver = std::make_shared<pe2d::PositionSolver>();
        m_World.AddSolver(solver);

        const pe2d::Transform mouseTracerTransform = pe2d::Transform(pe2d::Vector2(500.0f, 500.0f), pe2d::Vector2(1.0f, 1.0f), 0.0f);
        const pe2d::Vector2 mouseTracerSize = pe2d::Vector2(100.0f, 100.0f);
        AddBox(0U, sf::Color::Magenta, mouseTracerSize, mouseTracerTransform, false, true);

        const pe2d::Transform platformTransform = pe2d::Transform(pe2d::Vector2(500.0, 800.0f), pe2d::Vector2(1.0f, 1.0f), 0.0f);
        const pe2d::Vector2 platformSize = pe2d::Vector2(700.0f, 75.0f);
        AddBox(21U, sf::Color::White, platformSize, platformTransform, false, false);

        ResetVariables();        
    }

    void CollisionArenaTest::OnUpdate(float deltaTime, sf::Vector2i mousePos)
    {
        const auto objects = m_World.GetObjects();
        const pe2d::Vector2 s = objects.at(0)->GetPosition();
        const pe2d::Vector2 end = pe2d::Vector2{ (float)mousePos.x, (float)mousePos.y};
        const pe2d::Vector2 position = pe2d::Vector2::lerp(s, end, 10.0f * deltaTime);

        objects.at(0)->SetPosition(position);
        m_World.Step(deltaTime);  
    }

    void CollisionArenaTest::OnRender(sf::RenderWindow &window)
    {
        Draw(window);
    }

    void CollisionArenaTest::OnImGuiRender(sf::RenderWindow &window, const ImGuiIO &io)
    {
        if( ImGui::Button("Add Object") )
        {
            showObjectEditor = true;
        }
        if( ImGui::Button("Clear Objects") )
        {
            ClearObjects();
        }
        if(m_World.IsWorldPartitionizied())
        {
            if( ImGui::Button("Remove Partitioning System") )
            {
                m_World.RemovePartitiongSystem();
            }
        }
        else
        {
            if( ImGui::Button("Add Partitioning System") )
            {
                showPartitioningSystemEditor = true;;
            }
            if(showPartitioningSystemEditor)
            {
                ImGui::Begin("Partitinoning System");
                PartitioningSystemInput();
                ImGui::End();
            }
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

    void CollisionArenaTest::CollisionObjectInput()
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
        if(ImGui::Button("Is Movable"))
        {
            isMovable = !isMovable;
        }
    }
   
    void CollisionArenaTest::RigidObjectInput()
    {
        ImGui::InputFloat("Mass", &mass);
        ImGui::InputFloat2("Velocity", &velocity.x);
        ImGui::InputFloat2("Gravity", &gravity.x);
    } 

    void CollisionArenaTest::PartitioningSystemInput()
    {
        ImGui::InputFloat2("Top Left Corner", &topLeftCorner.x);
        ImGui::InputFloat2("Bottom Right Corner", &botRightCorner.x);
        ImGui::InputInt("Max Depth", &maxDepth);
        if( ImGui::Button("Add") )
        {
            m_World.SetPartitioningSystem(topLeftCorner, botRightCorner, maxDepth);
            showPartitioningSystemEditor = false;
        }
    }

    void CollisionArenaTest::ResetVariables()
    {
        isRigidObject = false;
        isMovable = true;
        ID = ID::BOX;
        size = pe2d::Vector2(0.0f, 0.0f);
        radius = 0.0f;
        position = pe2d::Vector2(500.0f, 500.0f);
        scale = pe2d::Vector2(1.0f, 1.0f);
        rotation = 0.0f;
        color = {255, 255, 255};
        mass = 0.0f;
        velocity = pe2d::Vector2(0.0f, 0.0f);
        gravity = pe2d::Vector2(0.0f, 0.0f);
        topLeftCorner = pe2d::Vector2(0.0f, 0.0f);
        botRightCorner = pe2d::Vector2(0.0f, 0.0f);
        maxDepth = 0;
    }

    void CollisionArenaTest::CreateObject()
    {
        const sf::Color Color = sf::Color{(sf::Uint8)color.red, (sf::Uint8)color.green, (sf::Uint8)color.blue};
        const pe2d::Transform transform = pe2d::Transform{ position, scale, rotation};
        const unsigned int ID = m_World.GetObjectsCount();
        if(ID == ID::BOX)
        {
            if(isRigidObject)
            {
                AddBox(ID, Color, size, transform, false, isMovable, mass, velocity, gravity);
            }
            else
            {
                AddBox(ID, Color, size, transform, false, isMovable);
            }
        }
        else if(ID == ID::CIRCLE)
        {
            if(isRigidObject)
            {
                AddCircle(ID, Color, radius, transform, false, isMovable, mass, velocity, gravity);
            }
            else
            {
                AddCircle(ID, Color, radius, transform, false, isMovable);
            }
        }
        showObjectEditor = false;
    }

    void CollisionArenaTest::ClearObjects()
    {
        auto objects = m_World.GetObjects();
        auto it = objects.begin();
        for(int i = 0; i < 2; i++) { it++; }
        for(;it!=objects.end();it++)
        {
            unsigned int ID = it->first;
            m_World.RemoveObject(ID);
        }
        for(int i = 2; i < m_Shapes.size(); i++)
        {
            m_Shapes.pop_back();
        }
    }
}   