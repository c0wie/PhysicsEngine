#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "Collision.hpp"
#include "Solver.hpp"
#include "BroadPhaseGrid.hpp"

namespace pe2d
{
    class CollisionWorld
    {
    public:
        CollisionWorld(bool IsWorldPartionized) : m_IsWorldPartionized(IsWorldPartionized),_Grid(nullptr) {}
        CollisionWorld(const CollisionWorld &other) = delete;
        CollisionWorld(CollisionWorld &&other) = delete;
        CollisionWorld operator = (const CollisionWorld &other) = delete;
        CollisionWorld operator = (CollisionWorld &&other) = delete;
        virtual ~CollisionWorld() = default;
    public:
        void AddCollisionObject(std::shared_ptr<CollisionObject> obj);
        void RemoveObject(std::shared_ptr<CollisionObject> object);

        void AddSolver(std::shared_ptr<Solver> &solver);
        void RemoveSolver(std::shared_ptr<Solver> &solver);
        
        void ResolveCollisions(float deltaTime);
        void SetBroadPhaseGrid(Vector2 topLeftCorner, Vector2 bottomRightCorner, float precision);
        inline const std::vector< std::shared_ptr<CollisionObject> >GetObjects() const { return m_Objects; }
        int GetObjectsCount() const { return m_Objects.size(); }
        constexpr bool IsWorldPartionized() { return m_IsWorldPartionized; }
    protected:
        void UpdateGrid();
        void FindCollisions(std::shared_ptr<CollisionObject> objectA, std::shared_ptr<CollisionObject> objectB,
            std::vector<Collision> &collisions, std::vector<Collision> &triggers);
        void SolveCollisions(std::vector<Collision> &collisions, float deltaTime);
        void SendCollisionCallbacks(std::vector<Collision> &collisions, float deltaTime);
    protected:
        std::vector< std::shared_ptr<CollisionObject> > m_Objects;
        std::vector< std::shared_ptr<Solver> > m_Solvers;
        bool m_IsWorldPartionized;
        std::unique_ptr<BroadPhaseGrid> _Grid;
    }; 
    
    class DynamicsWorld : public CollisionWorld
    {
    public:
        DynamicsWorld(bool IsWorldPartionized) : CollisionWorld(IsWorldPartionized) {}
        DynamicsWorld(const CollisionWorld &other) = delete;
        DynamicsWorld(DynamicsWorld &&other) = delete;
        DynamicsWorld operator=(const CollisionWorld &other) = delete;
        DynamicsWorld operator=(CollisionWorld &&other) = delete;
        ~DynamicsWorld() = default;
    public:
        void AddRigidObject(std::shared_ptr<RigidObject> object);
        void Step(float deltaTime);
    private:
        void ApplyGravity();
        void MoveObjects(float deltaTime);
    private:
        static constexpr float m_GRAVITY = 98.1f;
        static constexpr float m_MAX_VELOCITY = 1000.0f;
    };
}