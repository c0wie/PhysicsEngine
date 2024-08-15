#include "CollisionArenaTest.hpp"
#include "Algo.hpp"
namespace test
{
    Stoper stoper;
    CollisionArenaTest::CollisionArenaTest() :
        showObjectEditor(false)
    {   
        m_World.AddGrid(pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(1000.0f, 1000.0f), 100.0f);
        std::shared_ptr<pe2d::Solver> solver = std::make_shared<pe2d::PositionSolver>();
        m_World.AddSolver(solver);

        AddBox(420U, sf::Color::Red, pe2d::Vector2(1000.0f, 100.0f), pe2d::Transform(pe2d::Vector2(500.0f, 500.0f), pe2d::Vector2(1.0f, 1.0f), 30.0f),
                false, 100000000.0f, pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 0.0f), 1.0f, 1.0f, 0.0f);
        AddCircle(24U, sf::Color::Blue, 40.0f, pe2d::Transform(pe2d::Vector2(100.0f, 100.0f), pe2d::Vector2(1.0f, 1.0f), 0.0f),
                false, 200.0f, pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(0.0f, 0.0f), 1.0f, 1.0f, 0.0f);
        ResetVariables();        
    }

    void CollisionArenaTest::OnUpdate(float deltaTime, sf::Vector2i mousePos)
    {
        if(m_World.Size() != m_Shapes.size())
        {
            ASSERT("m_World size and m_Shapes size aren't the same");
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            auto object = std::static_pointer_cast<pe2d::RigidObject>(m_World.At(24));
            object->AddVelocity(pe2d::Vector2(100.0f, 0.0f) * deltaTime);
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            auto object = std::static_pointer_cast<pe2d::RigidObject>(m_World.At(24));
            object->AddVelocity(pe2d::Vector2(-100.0f, 0.0f) * deltaTime);
        }
        if(m_World.isColliding == !wasColliding && stoper.running == false)
        {
            stoper.start();
            wasColliding = m_World.isColliding;
        }
        if(m_World.isColliding == !wasColliding && stoper.running == true)
        {
            stoper.stop();
            stoper.printTime();
            wasColliding = m_World.isColliding;
        }
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
        auto obj = std::static_pointer_cast<pe2d::RigidObject>(m_World.At(24));
        const pe2d::Vector2 vel = obj->GetVelocity();
        ImGui::Text("Velocity: %i, %i", (int)vel.x, (int)vel.y);
        ImGui::Text("Number of objects: %i", m_World.Size());
        ImGui::Text("Application average %i ms/frame (%i FPS)", (int)(1000.0f / io.Framerate), (int)io.Framerate);

        if(showObjectEditor)
        {
            ImGui::Begin("Object Editor");
            CollisionObjectInput();
            if(isRigidObject)
            {
                if( ImGui::Button("Make Collision Object") )
                {
                    isRigidObject = false;
                }
            }
            else
            {
                if( ImGui::Button("Make Rigid Object") )
                {
                    isRigidObject = true;
                }
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
        ImGui::SliderFloat("Rotation", &rotation, -360.0f, 360.0f);
        if( ImGui::Button("Pick color") )
        {
            showColorPicker = !showColorPicker;
        }
        if(showColorPicker)
        {
            ImGui::ColorPicker3("Color", &color.red);
        }
        ImGui::SameLine();
         ImGui::ColorButton("##current_color", ImVec4(color.red, color.green, color.blue, 1.0f));
    }
   
    void CollisionArenaTest::RigidObjectInput()
    {
        ImGui::InputFloat("Mass", &mass);
        ImGui::InputFloat2("Velocity", &velocity.x);
        ImGui::InputFloat2("Gravity", &gravity.x);
        ImGui::SliderFloat("Static Friction", &staticFriction, 0.0f, 1.0f);
        ImGui::SliderFloat("DynamicFriction", &dynamicFriction, 0.0f, 1.0f);
    } 

    void CollisionArenaTest::ResetVariables()
    {
        showColorPicker = false;
        isRigidObject = false;
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
        staticFriction = 0.5f;
        dynamicFriction = 0.5f;
    }

    void CollisionArenaTest::CreateObject()
    {
        const sf::Color Color = sf::Color{(sf::Uint8)color.red, (sf::Uint8)color.green, (sf::Uint8)color.blue};
        const pe2d::Transform transform = pe2d::Transform{ position, scale, rotation};
        const unsigned int ID = m_World.Size();
        constexpr pe2d::Vector2 force = pe2d::Vector2(0.0f, 0.0f);
        if(objectType == ObjectType::BOX)
        {
            if(isRigidObject)
            {
                AddBox(ID, Color, size, transform, false, mass, force, velocity, gravity, staticFriction, dynamicFriction, 0.0f);
            }
            else
            {
                AddBox(ID, Color, size, transform, false);
            }
        }
        else if(objectType == ObjectType::CIRCLE)
        {
            if(isRigidObject)
            {
                AddCircle(ID, Color, radius, transform, false, mass, force, velocity, gravity, staticFriction, dynamicFriction, 0.0f);
            }
            else
            {
                AddCircle(ID, Color, radius, transform, false);
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