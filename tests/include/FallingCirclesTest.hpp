#pragma once

#include <vector>

#include <imgui-SFML.h>
#include <imgui.h>


#include "Test.hpp"
namespace test
{
    class FallingCirclesTest final : public Test
    {
    public:
        FallingCirclesTest();
        ~FallingCirclesTest() = default;
    public:
        void OnUpdate(float deltaTime, sf::Vector2i mousePos) override;
        void OnRender(sf::RenderWindow &window) override;
        void OnImGuiRender(sf::RenderWindow &window, const ImGuiIO &io) override;
    };
}