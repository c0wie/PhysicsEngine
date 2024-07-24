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
    };
    class TestCollision : public Test
    {
    public:
        TestCollision();
        ~TestCollision() = default;
    public:
        void OnUpdate(float deltaTime, sf::Vector2i mousePos) override final;
        void OnRender(sf::RenderWindow &window) override final;
        void OnImGuiRender(sf::RenderWindow &window, const ImGuiIO &io) override final;
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