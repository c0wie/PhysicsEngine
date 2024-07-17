#include "TestCollision.hpp"
#include "../../pe2d/include/Algo.hpp"
namespace test
{

    TestCollision::TestCollision()
    {
        pe2d::Transform transform = pe2d::Transform{ pe2d::Vector2{500.0f, 800.0f}, pe2d::Vector2{1.0f, 1.0f}, 0.0f };
        pe2d::Transform tracerTansform = pe2d::Transform{ pe2d::Vector2{500.0f, 500.0f}, pe2d::Vector2{1.0f, 1.0f}, 0.0f };
        AddBox(sf::Color::White, pe2d::Vector2{700.0f, 50.0f}, transform, false);
        AddBox(sf::Color::Cyan, pe2d::Vector2{100.0f, 100.f}, tracerTansform, false);

        std::shared_ptr<pe2d::Solver> solver = std::make_shared<pe2d::PositionSolver>();
        world.AddSolver(solver);
        showObjectEditor = false;
        ResetVariables();        
    }

    void TestCollision::OnUpdate(float deltaTime, const sf::Vector2i &mousePos)
    {
        const pe2d::Vector2 s = world.GetObjects()[1]->GetPosition();
        const pe2d::Vector2 end = pe2d::Vector2{ (float)mousePos.x, (float)mousePos.y};
        const pe2d::Vector2 position = pe2d::Vector2::lerp(s, end, 16.0f * deltaTime);

        world.GetObjects()[1]->SetPosition(position);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            world.GetObjects()[1]->Rotate(100.0f * deltaTime);
        }
        world.Step(deltaTime);  
    }

    void TestCollision::OnRender(sf::RenderWindow &window)
    {
        Draw(window);
    }

    void TestCollision::OnImGuiRender(sf::RenderWindow &window)
    {
        if( ImGui::Button("Add Object") )
        {
            showObjectEditor = true;
        }
        if( ImGui::Button("Clear Objects") )
        {
            ClearObjects();
        }
        ImGui::Text("Number of objects: %i", world.GetObjects().size());
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
        if(ID == ID::BOX)
        {
            if(isRigidObject)
            {
                AddBox(Color, size, transform, false, mass, velocity, gravity);
            }
            else
            {
                AddBox(Color, size, transform, false);
            }
        }
        else if(ID == ID::CIRCLE)
        {
            if(isRigidObject)
            {
                AddCircle(Color, radius, transform, false, mass, velocity, gravity);
            }
            else
            {
                AddCircle(Color, radius, transform, false);
            }
        }
        showObjectEditor = false;
    }
    
    void TestCollision::ClearObjects() 
    {
        auto objects = world.GetObjects();
        for(int i = 2; i < objects.size(); i++)
        {
            world.RemoveObject(objects[i]);
            m_Shapes.pop_back();
        }
    }
}   