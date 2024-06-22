#pragma once


#include <vector>
#include <memory>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "Collision.hpp"

namespace phy
{
    class PhysicsWorld
    {
    public:
        std::vector< std::shared_ptr<Object> > m_Objects;
        static inline constexpr float m_Gravity = 0.981f;
    public:
        PhysicsWorld() = default;
        ~PhysicsWorld() = default;
        PhysicsWorld(const PhysicsWorld &other) = delete;
        PhysicsWorld(PhysicsWorld &&other) = delete;
        PhysicsWorld operator = (const PhysicsWorld &other) = delete;
        PhysicsWorld operator = (PhysicsWorld &&other) = delete;

        void AddObject(const Object& obj);
        void RemoveObject(std::shared_ptr<Object> object);
        void Step(float deltaTime);
        void ResolveCollision(float delatTime);
        void Draw(sf::RenderWindow &window);
    }; 
}