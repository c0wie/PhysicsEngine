#pragma once

#include <vector>

#include <imgui-SFML.h>
#include <imgui.h>


#include "Test.hpp"
namespace test
{
    struct Color
    {
        int red, green, blue;
    };
    enum ID
    {
        BOX, 
        CIRCLE,
        CONVEXSHAPE
    };
    class TestCollision : public Test
    {
    public:
        TestCollision();
        ~TestCollision() { delete[] vertecies; }
    public:
        void OnUpdate(float deltaTime, const sf::Vector2i &mousePos) override final;
        void OnRender(sf::RenderWindow &window) override final;
        void OnImGuiRender(sf::RenderWindow &window) override final;
    private:
        void CollisionObjectInput();
        void RigidObjectInput();
        void ResetVariables();
        void CreateObject();
    private:
        bool showObjectEditor;
        bool isRigidObject;
        ID ID;
        float radius;
        int verteciesCount;
        pe2d::Vector2 *vertecies;
        pe2d::Vector2 size;
        pe2d::Vector2 position;
        pe2d::Vector2 scale;
        float rotation;
        float mass;
        Color color;
        pe2d::Vector2 velocity;
        pe2d::Vector2 gravity;
    };
}