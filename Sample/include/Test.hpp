#pragma once

#include "Shape.hpp"
#include "PhysicsWorld.hpp"

#include <imgui.h>

namespace test
{
    class Test
    {
    public:
        Test() = default;
        virtual ~Test() = default;
    public:
        virtual void OnUpdate(float deltaTime, sf::Vector2i mousePos) = 0;
        virtual void OnRender(sf::RenderWindow &window) = 0;
        virtual void OnImGuiRender(sf::RenderWindow &window, const ImGuiIO& io) = 0;
    protected:
        void AddCircle(const sf::Color color, pe2d::RigidObject &object);
        void AddCircle(size_t ID, const sf::Color &color, float radius, pe2d::Transform transform, bool isStatic,
                        float mass, pe2d::Vector2 velocity,pe2d::Vector2 gravity, float staticFriction,
                        float dynamicFriction, float restitution);
        void AddBox(const sf::Color color, pe2d::RigidObject &object);
        void AddBox(unsigned int ID, const sf::Color &color, pe2d::Vector2 size, pe2d::Transform transform, bool isStatic,
                    float mass, pe2d::Vector2 velocity, pe2d::Vector2 gravity, float staticFriction,
                    float dynamicFriction, float restitution);
        
        void Draw(sf::RenderWindow &window) const;
    protected:
        std::vector<Shape> m_Shapes;
        pe2d::PhysicsWorld m_World;
    };
    void DrawCircle(sf::RenderWindow &window, const Shape &shape);
    void DrawBox(sf::RenderWindow &window, const Shape &shape);
}