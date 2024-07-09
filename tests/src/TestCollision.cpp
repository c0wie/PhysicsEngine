#include "TestCollision.hpp"

namespace test
{
    struct Color
    {
        
       int red, green, blue;
    };

    TestCollision::TestCollision()
    {
        std::shared_ptr<pe2d::Solver> solver = std::make_shared<pe2d::PositionSolver>();
        world.AddSolver(solver);
        m_Vertecies = new pe2d::Vector2[3];
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
        static bool showObjectEditor;
        static bool isRigidObject;
        static std::string name = "Square";
        static float radius = 10.0f;
        static int verteciesCount = 3;
        static pe2d::Vector2 size = pe2d::Vector2{10.0f, 10.0f};
        static pe2d::Vector2 position;
        static pe2d::Vector2 scale = pe2d::Vector2{1.0f, 1.0f};
        static float rotation;
        static float mass;
        static Color color = {255, 255, 255};
        static pe2d::Vector2 velocity;
        static pe2d::Vector2 gravity;

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
            if(name == "Square")
            {
                if( ImGui::Button("Make Circle") )
                {
                    name = "Circle";
                }
                ImGui::SameLine();
                if( ImGui::Button("Make Convex Shape") )
                {
                    name = "Convex Shape";
                }
                ImGui::InputFloat2("Size", &size.x);
            }
            else if(name == "Circle")
            {
                if( ImGui::Button("Make Square") )
                {
                  name = "Square";
                }
                ImGui::SameLine();
                if( ImGui::Button("Make Convex Shape") )
                {
                    name = "Convex Shape";
                }
                ImGui::InputFloat("Radius", &radius);
            }
            else
            {
                if( ImGui::Button("Make Square") )
                {
                  name = "Square";
                }
                ImGui::SameLine();
                if( ImGui::Button("Make Circle") )
                {
                    name = "Circle";
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
                        m_Vertecies = new pe2d::Vector2[verteciesCount];
                    }
                }
                for(int i = 0; i< verteciesCount; i++)
                {
                    std::string label = "Vertex" + std::to_string(i);
                    ImGui::InputFloat2(label.c_str(), &m_Vertecies[i].x);
                }
            }
            if(name != "Convex Shape")
            {
                ImGui::InputFloat2("Position", &position.x);
            }
            ImGui::InputFloat2("Scale", &scale.x);
            ImGui::InputFloat("Rotation", &rotation);
            ImGui::InputInt3("Color", &color.red);
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
                ImGui::InputFloat("Mass", &mass);
                ImGui::InputFloat2("Velocity", &velocity.x);
                ImGui::InputFloat2("Gravity", &gravity.x);
            }
            if( ImGui::Button("Reset Variables") )
            {
                isRigidObject = false;
                name = "Square";
                verteciesCount = 3;
                m_Vertecies = new pe2d::Vector2[verteciesCount];;
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
            
            if( ImGui::Button("Create") )
            {
                const sf::Color Color = sf::Color{(sf::Uint8)color.red, (sf::Uint8)color.green, (sf::Uint8)color.blue};
                const pe2d::Transform transform = pe2d::Transform{ position, scale, rotation};
                if(name == "Square")
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
                else if(name == "Circle" )
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
                else if(name == "Convex Shape")
                {
                    if(isRigidObject)
                    {
                        AddConvexShape(Color, verteciesCount, m_Vertecies, transform, false);
                    }
                    else
                    {
                        AddConvexShape(Color, verteciesCount, m_Vertecies, transform, false, mass, velocity, gravity);
                    }
                }
                showObjectEditor = false;
            }
            ImGui::End();
        }
    }
}   