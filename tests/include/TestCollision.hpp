#pragma once

#include <vector>

#include <imgui-SFML.h>
#include <imgui.h>


#include "Test.hpp"
namespace test
{
    class TestCollision : public Test
    {
        public:
            TestCollision();
            ~TestCollision() = default;
        public:
            void OnUpdate(float deltaTime, const sf::Vector2i &mousePos) override final;
            void OnRender(sf::RenderWindow &window) override final;
            void OnImGuiRender(sf::RenderWindow &window) override final;
    };
}