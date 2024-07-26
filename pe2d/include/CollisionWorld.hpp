#pragma once

#include <unordered_map>
#include <memory>
#include <algorithm>

#include "Collision.hpp"
#include "Solver.hpp"
#include "QuadTree.hpp"

namespace pe2d
{
    // class that run all collision checks and own all items 
    // you call it to use physics engine
    class CollisionWorld
    {
    public:
        CollisionWorld() = default;
        CollisionWorld(const CollisionWorld &other) = delete;
        CollisionWorld(CollisionWorld &&other) = delete;
        CollisionWorld operator = (const CollisionWorld &other) = delete;
        CollisionWorld operator = (CollisionWorld &&other) = delete;
        virtual ~CollisionWorld() = default;
    public:
        void AddCollisionObject(std::shared_ptr<CollisionObject> obj);
        void RemoveObject(unsigned int ID);

        void AddSolver(std::shared_ptr<Solver> &solver);
        void RemoveSolver(std::shared_ptr<Solver> &solver);
        
        void SetPartitioningSystem(Vector2 topLeftCorner, Vector2 bottomRightCorner, unsigned int maxDepth);
        void RemovePartitiongSystem();
        
        void ResolveCollisions(float deltaTime);
        std::unordered_map<unsigned int, std::shared_ptr<CollisionObject>> GetObjects() const { return m_Objects; }
        void ClearObjects() { m_Objects.clear(); }
        
        unsigned int GetObjectsCount() const { return m_Objects.size(); }
        constexpr bool IsWorldPartitionizied() { return m_IsPartitioningSystemOn; }
    protected:
        void UpdateGrid();
        void FindCollisions(std::shared_ptr<CollisionObject> objectA, std::shared_ptr<CollisionObject> objectB,
            std::vector<Collision> &collisions, std::vector<Collision> &triggers);
        void SolveCollisions(std::vector<Collision> &collisions, float deltaTime);
        void SendCollisionCallbacks(std::vector<Collision> &collisions, float deltaTime);
    protected:
        std::unordered_map<unsigned int, std::shared_ptr<CollisionObject>> m_Objects;
        std::vector<std::shared_ptr<Solver>> m_Solvers;
        bool m_IsPartitioningSystemOn;
        StaticQuadTreeContainer<std::shared_ptr<CollisionObject>> m_PartitioningSystem;
    }; 
}