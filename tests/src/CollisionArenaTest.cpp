#include "CollisionArenaTest.hpp"

namespace test
{
    CollisionArenaTest::CollisionArenaTest() :
        showObjectEditor(false)
    {
        m_World.AddGrid(pe2d::Vector2(-100, -100), pe2d::Vector2(1100, 1100), 200.0f);
        std::shared_ptr<pe2d::Solver> solver = std::make_shared<pe2d::PositionSolver>();
        m_World.AddSolver(solver);

        const pe2d::Transform mouseTracerTransform = pe2d::Transform(pe2d::Vector2(500.0f, 500.0f), pe2d::Vector2(1.0f, 1.0f), 0.0f);
        const pe2d::Vector2 mouseTracerSize = pe2d::Vector2(100.0f, 100.0f);
        AddBox(2137U, sf::Color::Magenta, mouseTracerSize, mouseTracerTransform, false, true);

        const pe2d::Transform platformTransform = pe2d::Transform(pe2d::Vector2(500.0, 800.0f), pe2d::Vector2(1.0f, 1.0f), 0.0f);
        const pe2d::Vector2 platformSize = pe2d::Vector2(600.0f, 75.0f);
        AddBox(210U, sf::Color::White, platformSize, platformTransform, false, false);

        ResetVariables();        
    }

    void CollisionArenaTest::OnUpdate(float deltaTime, sf::Vector2i mousePos)
    {
        if(m_World.Size() != m_Shapes.size())
        {
            ASSERT("m_World size and m_Shapes size aren't the same");
        }
        auto mouseTracer = m_World.At(2137);
        const pe2d::Vector2 s = mouseTracer->GetPosition();
        const pe2d::Vector2 end = pe2d::Vector2{ (float)mousePos.x, (float)mousePos.y};
        const pe2d::Vector2 position = pe2d::Vector2::lerp(s, end, 10.0f * deltaTime);

        mouseTracer->SetPosition(position);
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
        ImGui::Text("Number of objects: %i", m_World.Size());
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
        if(objectType == ObjectType::BOX)
        {
            if( ImGui::Button("Make Circle") )
            {
                objectType = ObjectType::CIRCLE;
            }
            ImGui::InputFloat2("Size", &size.x);
        }
        else if(objectType == ObjectType::CIRCLE)
        {
            if( ImGui::Button("Make Square") )
            {
                objectType = ObjectType::BOX;
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

    void CollisionArenaTest::ResetVariables()
    {
        isRigidObject = false;
        isMovable = true;
        objectType = ObjectType::BOX;
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
        const unsigned int ID = m_World.Size();
        if(objectType == ObjectType::BOX)
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
        else if(objectType == ObjectType::CIRCLE)
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
        else
        {
            ASSERT("Unvalid ID for object");
        }
        showObjectEditor = false;
    }

    void CollisionArenaTest::ClearObjects()
    {
        for(auto it = m_World.Begin(); it != m_World.End();)
        {
            if(it->first == 210 || it->first == 2137)
            {
                it++;
            }
            else
            {
                it = m_World.RemoveObject(it);
            }
        }
        if(m_Shapes.size() > 2)
        {
            m_Shapes.erase(std::next(m_Shapes.begin(), 2), m_Shapes.end());
        }
    }
}   