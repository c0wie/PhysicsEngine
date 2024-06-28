#pragma once


#include <vector>
#include <memory>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "Collision.hpp"
#include "Solver.hpp"

namespace phy
{
    class CollisionWorld
    {
    // TODO: when I finish debugging I heve to make it protected
    public:
        std::vector< std::shared_ptr<CollisionObject> > m_Objects;
        std::vector< std::shared_ptr<Solver> > m_Solvers;
    private:
        void SolveCollisions(std::vector<Collision> &collisions, float deltaTime);
        void SendCollisionCallbacks(std::vector<Collision> &collisions, float deltaTime);
    public:
        CollisionWorld() = default;
        CollisionWorld(const CollisionWorld &other) = delete;
        CollisionWorld(CollisionWorld &&other) = delete;
        CollisionWorld operator = (const CollisionWorld &other) = delete;
        CollisionWorld operator = (CollisionWorld &&other) = delete;
        virtual ~CollisionWorld() = default;

        void AddCollisionObject(std::shared_ptr<CollisionObject> obj);
        void RemoveObject(std::shared_ptr<CollisionObject> object);

        void AddSolver(std::shared_ptr<Solver> &solver);
        void AddSolver(std::shared_ptr<PositionSolver> &solver);
        void AddSolver(std::shared_ptr<ImpulseSolver> &solver);
        void RemoveSolver(std::shared_ptr<Solver> &solver);
        
        void ResolveCollisions(float deltaTime);
        void Draw(sf::RenderWindow &window);
    }; 
    class DynamicsWorld : public CollisionWorld
    {
    private:
        static constexpr float m_Gravity = 98.1f;
    private:
        void ApplyGravity();
        void MoveObjects(float deltaTime);
    public:
        DynamicsWorld() = default;
        DynamicsWorld(const CollisionWorld &other) = delete;
        DynamicsWorld(DynamicsWorld &&other) = delete;
        DynamicsWorld operator=(const CollisionWorld &other) = delete;
        DynamicsWorld operator=(CollisionWorld &&other) = delete;
        ~DynamicsWorld() = default;

        void AddRigidObject(std::shared_ptr<RigidObject> object);
        void Step(float deltaTime);
    };
}