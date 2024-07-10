#include "TestCollision.hpp"

namespace test
{

    TestCollision::TestCollision()
    {
        pe2d::Transform transform = pe2d::Transform{ pe2d::Vector2{500.0f, 950.0f}, pe2d::Vector2{1.0f, 1.0f}, 0.0f };
        AddBox(sf::Color::White, pe2d::Vector2{1000.0f, 50.0f}, transform, false);
        std::shared_ptr<pe2d::Solver> solver = std::make_shared<pe2d::PositionSolver>();
        world.AddSolver(solver);
        showObjectEditor = false;
        ResetVariables();        
    }

    void TestCollision::OnUpdate(float deltaTime, const sf::Vector2i &mousePos)
    {
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
            ImGui::SameLine();
            if( ImGui::Button("Make Convex Shape") )
            {
                ID = ID::CONVEXSHAPE;
            }
            ImGui::InputFloat2("Size", &size.x);
        }
        else if(ID == ID::CIRCLE)
        {
            if( ImGui::Button("Make Square") )
            {
                ID = ID::BOX;
            }
            ImGui::SameLine();
            if( ImGui::Button("Make Convex Shape") )
            {
                ID = ID::CONVEXSHAPE;
            }
            ImGui::InputFloat("Radius", &radius);
        }
        else
        {
            if( ImGui::Button("Make Square") )
            {
                ID = ID::BOX;
            }
            ImGui::SameLine();
            if( ImGui::Button("Make Circle") )
            {
                ID = ID::CIRCLE;
            }
            if( ImGui::InputInt("Vertecies count", &verteciesCount) )
            {
                if(verteciesCount < 3)
                {
                    verteciesCount = 3;
                }
                else if(verteciesCount > 10)
                {
                    verteciesCount = 10;
                }
                else
                {
                    vertecies = new pe2d::Vector2[verteciesCount];
                }
            }
            for(int i = 0; i< verteciesCount; i++)
            {
                std::string label = "Vertex" + std::to_string(i);
                ImGui::InputFloat2(label.c_str(), &vertecies[i].x);
            }
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
        verteciesCount = 3;
        vertecies = new pe2d::Vector2[verteciesCount];
        size = pe2d::Vector2{};
        radius = 0.0f;
        position = pe2d::Vector2{};
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
        else if(ID == ID::CONVEXSHAPE)
        {
            if(isRigidObject)
            {
                AddConvexShape(Color, verteciesCount, vertecies, transform, false);
            }
            else
            {
                AddConvexShape(Color, verteciesCount, vertecies, transform, false, mass, velocity, gravity);
            }
        }
        showObjectEditor = false;
    }
    void TestCollision::ClearObjects() 
    {
        while(m_Shapes.size() != 1)
        {
            m_Shapes.pop_back();
        }
        auto objects = world.GetObjects();
        for(int i = 1; i < objects.size(); i++)
        {
            world.RemoveObject(objects[i]);
        }
    }
}   