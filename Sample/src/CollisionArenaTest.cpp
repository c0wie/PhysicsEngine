#include "../include/CollisionArenaTest.hpp"
#include "Algo.hpp"
#include <random>
namespace test
{   
    CollisionArenaTest::CollisionArenaTest() :
        m_TimeSinceLastSpawn(0.0f),
        showObjectEditor(false)
    {   
        m_World.AddGrid(pe2d::Vector2(0.0f, 0.0f), pe2d::Vector2(1000.0f, 1000.0f), 100.0f);
        m_World.SetSolver(ImpulseSolverWithoutFriction);

        AddBox(2217U, sf::Color::Green, {800.0f, 100.0f}, pe2d::Transform({500.0f, 800.0f}, {1.0f, 1.0f}, pe2dMath::DeegresToRadians(0.0f)),
                true, 10.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, 1.0f, 1.0f, 0.0f);
        AddBox(2137U, sf::Color::Green, {300.0f, 50.0f}, pe2d::Transform({300.0f, 400.0f}, {1.0f, 1.0f}, pe2dMath::DeegresToRadians(45.0f)),
                true, 10.0f, {0.0f, 0.0f}, {0.0f, 0.0f}, 1.0f, 1.0f, 0.0f);
        AddBox(3U, sf::Color::Blue, {100.0f, 100.0f}, pe2d::Transform({700.0f, 500.0f}, {1.0f, 1.0f}, pe2dMath::DeegresToRadians(0.0f)),
                false, 100.0f, {0.0f, 0.0f}, {0.0f, 98.1f}, 1.0f, 1.0f, 0.0f);
        AddBox(2U, sf::Color::Blue, {100.0f, 100.0f}, pe2d::Transform({760.0f, 100.0f}, {1.0f, 1.0f}, pe2dMath::DeegresToRadians(0.0f)),
                false, 100.0f, {0.0f, 0.0f}, {0.0f, 98.1f}, 1.0f, 1.0f, 0.0f);
        ResetVariables();        
    }

    void CollisionArenaTest::OnUpdate(float deltaTime, sf::Vector2i mousePos)
    {
        m_TimeSinceLastSpawn += deltaTime;
        if(m_World.Size() != m_Shapes.size())
        {
            ASSERT("m_World size and m_Shapes size aren't the same");
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_TimeSinceLastSpawn >= m_SquareSpawnCooldown)
        {
            m_TimeSinceLastSpawn = 0.0f;
            pe2d::RigidObject obj = GetRandomBox(m_World.Size(), pe2d::Vector2(mousePos.x, mousePos.y));
            AddBox(sf::Color::Blue, obj);
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && m_TimeSinceLastSpawn >= m_SquareSpawnCooldown)
        {
            m_TimeSinceLastSpawn = 0.0f;
            pe2d::RigidObject obj = GetRandomCircle(m_World.Size(), pe2d::Vector2(mousePos.x, mousePos.y));
            AddCircle(sf::Color::Red, obj);
        }
        m_World.Step(deltaTime);  
    }

    void CollisionArenaTest::OnRender(sf::RenderWindow &window)
    {
        Draw(window);
    }

    void CollisionArenaTest::OnImGuiRender(sf::RenderWindow &window, const ImGuiIO &io)
    {
        if(m_World.Size() > 0)
        {
            const Vector2 linearVelocityA = m_World.At(3).GetLinearVelocity();
            const float angularVelocityA = m_World.At(3).GetAngularVelocity();
            const Vector2 linearVelocityB = m_World.At(2).GetLinearVelocity();
            const float angularVelocityB = m_World.At(2).GetAngularVelocity();
            ImGui::Text("Linear velocityA: %i, %i", (int)(linearVelocityA.x), (int)(linearVelocityA.y));
            ImGui::Text("Angular velocityA: %i", (int)(angularVelocityA));
            ImGui::Text("Linear velocityA: %i, %i", (int)(linearVelocityB.x), (int)(linearVelocityB.y));
            ImGui::Text("Angular velocityA: %i", (int)(angularVelocityB));
        }
        ImGui::Text("Application average %i ms/frame (%i FPS)", (int)(1000.0f / io.Framerate), (int)io.Framerate);
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
        ImGui::InputFloat("Mass", &mass);
        ImGui::InputFloat2("Velocity", &velocity.x);
        ImGui::InputFloat2("Gravity", &gravity.x);
        ImGui::SliderFloat("Static Friction", &staticFriction, 0.0f, 1.0f);
        ImGui::SliderFloat("DynamicFriction", &dynamicFriction, 0.0f, 1.0f);
    }
   
    void CollisionArenaTest::ResetVariables()
    {
        showColorPicker = false;
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
            AddBox(ID, Color, size, transform, false, mass, velocity, gravity, staticFriction, dynamicFriction, 0.0f);
        }
        else if(objectType == ObjectType::CIRCLE)
        {
            AddCircle(ID, Color, radius, transform, false, mass, velocity, gravity, staticFriction, dynamicFriction, 0.0f);
        }
        else
        {
            ASSERT("Unvalid ID for object");
        }
        showObjectEditor = false;
    }

    void CollisionArenaTest::ClearObjects()
    {
        m_Shapes.clear();
        m_World.ClearObjects();
    }

    pe2d::RigidObject GetRandomBox(size_t ID, pe2d::Vector2 pos)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribInt(30, 150);
        std::uniform_real_distribution<float> distribFloat(0.0f, 1.0f);
        std::uniform_int_distribution<int> distribPos(200, 800);
        std::uniform_int_distribution<int> distribRotation(0, 180);
        float mass = 100.f;
        pe2d::Transform transform = pe2d::Transform(pos, pe2d::Vector2(1.0f, 1.0f), 0.0f);
        pe2d::Vector2 size = pe2d::Vector2(100.0f, 100.0f);
        float staticFriction = 1.0f;
        float dynamicFriction = 1.0f;
        float resistance = 0.0f;
        pe2d::RigidObject object(ID, std::make_shared<pe2d::BoxCollider>(size), transform, mass, {0.0f, 98.1f}, false,
                        staticFriction, dynamicFriction, resistance);
        return object;
    }

    pe2d::RigidObject GetRandomCircle(size_t ID, pe2d::Vector2 pos)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribInt(30, 70);
        std::uniform_real_distribution<float> distribFloat(0.0f, 1.0f);
        std::uniform_int_distribution<int> distribPos(200, 800);
        std::uniform_int_distribution<int> distribRotation(0, 360);
        const pe2d::Transform transform = pe2d::Transform(pos, pe2d::Vector2(1.0f, 1.0f), 0.0f);
        float mass = 1.0f;
        float radius = 50.0f;
        float staticFriction = 1.0f;
        float dynamicFriction = 1.0f;
        float resistance = 0.0f;
        std::shared_ptr<pe2d::CircleCollider> collider = std::make_shared<pe2d::CircleCollider>(radius);
        pe2d::RigidObject object(ID, collider, transform, mass, pe2d::Vector2(0.0f, 98.1f), false,
                        staticFriction, dynamicFriction, resistance);
        return object;
    }
}   