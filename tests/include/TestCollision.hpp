#pragma once

#include <vector>

#include <imgui-SFML.h>
#include <imgui.h>
#include "../../pe2d/include/PhysicsWorld.hpp"


#include "Test.hpp"
namespace test
{
    class TestCollision : public Test
    {
        public:
            TestCollision();
        public:
            void OnUpdate(float deltaTime) override final;
            void OnRender(sf::RenderWindow &window) override final;
            void OnImGuiRender(sf::RenderWindow &window, const sf::Vector2i &mousePosition) override final;
        private:
            std::shared_ptr<pe2d::CollisionObject> CreateCollisionObject(const std::string &type, float size, const pe2d::Vector2 &position,
            const pe2d::Vector2 &scale = pe2d::Vector2{1.0f, 1.0f}, float rotation = 0.0f, bool trigger = false) override final;

            std::shared_ptr<pe2d::RigidObject> CreateRigidObject(const std::string &type, float size, float mass, const pe2d::Vector2 &position,
            const pe2d::Vector2 &velocity = pe2d::Vector2{0.0f, 0.0f}, const pe2d::Vector2 &scale = pe2d::Vector2{1.0f, 1.0f},
            float rotation = 0.0f, bool trigger = false, const pe2d::Vector2 &gravity = pe2d::Vector2{0.0f, 9.81f},
            bool takesGravity = false, float staticFriction = 0.0f, float dynamicFriction = 0.0f, float restitution = 0.0f) override final;

            void Draw(sf::RenderWindow &window, const std::shared_ptr<pe2d::CollisionObject> obj) const override final;
        public:
            pe2d::DynamicsWorld world;
        private:
            bool showObjectEditor;
    };
}