#pragma once

#include "Shape.hpp"
#include <imgui.h>
#include "../../pe2d/include/DynamicsWorld.hpp"


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
        void AddCircle(unsigned int ID, const sf::Color &color, float radius, pe2d::Transform transform, bool isTrigger);
        void AddCircle(unsigned int ID, const sf::Color &color, float radius, pe2d::Transform transform, bool isTrigger,
                        float mass, pe2d::Vector2 velocity, pe2d::Vector2 force, pe2d::Vector2 gravity, float staticFriction,
                        float dynamicFriction, float bounciness);
        void AddCircle(unsigned int ID, const sf::Color &color, float radius, pe2d::Transform transform, bool isTrigger,
                        float mass, pe2d::Vector2 velocity, pe2d::Vector2 force, float staticFriction, float dynamicFriction, float bounciness);
        void AddBox(unsigned int ID, const sf::Color &color, pe2d::Vector2 size, pe2d::Transform transform, bool isTrigger);
        void AddBox(unsigned int ID, const sf::Color &color, pe2d::Vector2 size, pe2d::Transform transform, bool isTrigger,
                    float mass, pe2d::Vector2 velocity, pe2d::Vector2 force, pe2d::Vector2 gravity, float staticFriction,
                    float dynamicFriction, float bounciness);
        void AddBox(unsigned int ID, const sf::Color &color, pe2d::Vector2 size, pe2d::Transform transform, bool isTrigger,
                    float mass, pe2d::Vector2 velocity, pe2d::Vector2 force, float staticFriction, float dynamicFriction, float bounciness);
        
        void Draw(sf::RenderWindow &window) const;
    private:
        static void DrawCircle(sf::RenderWindow &window, const Shape &shape);
        static void DrawBox(sf::RenderWindow &window, const Shape &shape);
    protected:
        std::vector<Shape> m_Shapes;
        pe2d::DynamicsWorld m_World;
    };
}