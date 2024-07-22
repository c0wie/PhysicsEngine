#pragma once

#include "Shape.hpp"
#include <imgui.h>
#include "../../pe2d/include/PhysicsWorld.hpp"


namespace test
{
    class Test
    {
    public:
        Test(bool IsWorldPartionized) : world(IsWorldPartionized) {}
        ~Test() = default;
    public:
        virtual void OnUpdate(float deltaTime, const sf::Vector2i &mousePos) = 0;
        virtual void OnRender(sf::RenderWindow &window) = 0;
        virtual void OnImGuiRender(sf::RenderWindow &window, const ImGuiIO& io) = 0;
    protected:
        void AddCircle(unsigned int ID, const sf::Color &color, float radius, const pe2d::Transform &transform, bool isTrigger);
        void AddCircle(unsigned int ID, const sf::Color &color, float radius, const pe2d::Transform &transform, bool isTrigger, float mass, pe2d::Vector2 velocity, pe2d::Vector2 gravity);
        void AddBox(unsigned int ID, const sf::Color &color, pe2d::Vector2 size, const pe2d::Transform &transform, bool isTrigger);
        void AddBox(unsigned int ID, const sf::Color &color, pe2d::Vector2 size, const pe2d::Transform &transform, bool isTrigger, float mass, pe2d::Vector2 velocity, pe2d::Vector2 gravity);
        
        void Draw(sf::RenderWindow &window) const;
    private:
        static void DrawCircle(sf::RenderWindow &window, const Shape &shape);
        static void DrawBox(sf::RenderWindow &window, const Shape &shape);
    protected:
        std::vector<Shape> m_Shapes;
        pe2d::DynamicsWorld world;
    };
}