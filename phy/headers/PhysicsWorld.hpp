#pragma once


#include <vector>
#include <memory>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "Collision.hpp"
#include "Solver.hpp"

namespace phy
{
    class PhysicsWorld
    {
    public:
        std::vector< std::shared_ptr<CollisionObject> > m_Objects;
        std::vector< std::shared_ptr<Solver> > m_Solvers;
        static inline constexpr float m_Gravity = 0.00981f;
    public:
        PhysicsWorld() = default;
        ~PhysicsWorld() = default;
        PhysicsWorld(const PhysicsWorld &other) = delete;
        PhysicsWorld(PhysicsWorld &&other) = delete;
        PhysicsWorld operator = (const PhysicsWorld &other) = delete;
        PhysicsWorld operator = (PhysicsWorld &&other) = delete;

        void AddObject(const std::shared_ptr<CollisionObject>& obj);
        void RemoveObject(std::shared_ptr<CollisionObject> object);
        void AddSolver(const std::shared_ptr<Solver> &solver);
        void RemoveSolver(const std::shared_ptr<Solver> &solver);
        void Step(float deltaTime);
        void ResolveCollision(float delatTime);
        void Draw(sf::RenderWindow &window);
    }; 
}