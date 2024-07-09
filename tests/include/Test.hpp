#pragma once

#include "Shape.hpp"
#include "../../pe2d/include/PhysicsWorld.hpp"


namespace test
{
    class Test
    {
    public:
        Test() = default;
        ~Test() = default;
    public:
        virtual void OnUpdate(float deltaTime, const sf::Vector2i &mousePos) = 0;
        virtual void OnRender(sf::RenderWindow &window) = 0;
        virtual void OnImGuiRender(sf::RenderWindow &window) = 0;
    protected:
        void AddCircle(const sf::Color &color, float radius, const pe2d::Transform &transform, bool isTrigger);
        void AddCircle(const sf::Color &color, float radius, const pe2d::Transform &transform, bool isTrigger, float mass, const pe2d::Vector2 &velocity, const pe2d::Vector2 &gravity);
        void AddBox(const sf::Color &color, const pe2d::Vector2 &size, const pe2d::Transform &transform, bool isTrigger);
        void AddBox(const sf::Color &color, const pe2d::Vector2 &size, const pe2d::Transform &transform, bool isTrigger, float mass, const pe2d::Vector2 &velocity, const pe2d::Vector2 &gravity);
        void AddConvexShape(const sf::Color &color, unsigned int verteciesCount, pe2d::Vector2 *vertecies, const pe2d::Transform &transform, bool isTrigger);
        void AddConvexShape(const sf::Color &color, unsigned int verteciesCount, pe2d::Vector2 *vertecies, const pe2d::Transform &transform,
            bool isTrigger, float mass, const pe2d::Vector2 &velocity, const pe2d::Vector2 &gravity);
        
        void Draw(sf::RenderWindow &window) const;
        virtual void ClearObjects(); 
    private:
        static void DrawCircle(sf::RenderWindow &window, const Shape &shape);
        static void DrawBox(sf::RenderWindow &window, const Shape &shape);
        static void DrawConvexShape(sf::RenderWindow &window, const Shape &shape);
    protected:
        std::vector<Shape> m_Shapes;
        pe2d::DynamicsWorld world;
    };
}