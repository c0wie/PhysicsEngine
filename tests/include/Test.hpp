#pragma once

#include <SFML/Graphics.hpp>
#include "../../pe2d/include/RigidObject.hpp"
namespace test
{
    class Test
    {
    public:
        Test() = default;
    public:
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnRender(sf::RenderWindow &window) = 0;
        virtual void OnImGuiRender(sf::RenderWindow &window, const sf::Vector2i &mousePosition) = 0;
    protected:
        virtual std::shared_ptr<pe2d::CollisionObject> CreateCollisionObject(const std::string &type, float size, const pe2d::Vector2 &position,
            const pe2d::Vector2 &scale, float rotation, bool trigger) = 0;

        virtual std::shared_ptr<pe2d::RigidObject> CreateRigidObject(const std::string &type, float size, float mass, const pe2d::Vector2 &position,
            const pe2d::Vector2 &velocity, const pe2d::Vector2 &scale, float rotation, bool trigger, const pe2d::Vector2 &gravity, bool takesGravity,
            float staticFriction, float dynamicFriction, float restitution) = 0;

        virtual void Draw(sf::RenderWindow &window, const std::shared_ptr<pe2d::CollisionObject> obj) const  = 0;
    };
}