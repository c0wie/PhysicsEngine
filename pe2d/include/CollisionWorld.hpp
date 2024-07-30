#pragma once

#include <unordered_map>
#include <memory>
#include <algorithm>

#include "Collision.hpp"
#include "Solver.hpp"
#include "QuadTreeContainer.hpp"

namespace pe2d
{
    // class that run all collision checks and own all items 
    // you call it to use physics engine
    class CollisionWorld
    {
    public:
        CollisionWorld(Vector2 topLeftCorner, Vector2 bottomRightCorner, unsigned int maxDepth) :
            m_Objects(topLeftCorner, bottomRightCorner, maxDepth)
        {}
        CollisionWorld(const CollisionWorld &other) = delete;
        CollisionWorld(CollisionWorld &&other) = delete;
        CollisionWorld operator = (const CollisionWorld &other) = delete;
        CollisionWorld operator = (CollisionWorld &&other) = delete;
        virtual ~CollisionWorld() = default;
    public:
        void AddObject(std::shared_ptr<CollisionObject> object);
        void RemoveObject(std::list<QuadTreeItem<std::shared_ptr<CollisionObject>>>::iterator object) { m_Objects.Remove(object); }

        void AddSolver(std::shared_ptr<Solver> &solver);
        void RemoveSolver(std::shared_ptr<Solver> &solver);
        
        void Resize(Vector2 topLeftCorner, Vector2 bottomRightCorner);
        
        void ResolveCollisions(float deltaTime);
        void ClearObjects() { m_Objects.Clear(); }
        
        size_t Size() const { return m_Objects.Size(); }
        bool Empty() const { return m_Objects.Empty(); }
        std::list<QuadTreeItem<std::shared_ptr<CollisionObject>>>::iterator Begin() { return m_Objects.Begin(); }
        std::list<QuadTreeItem<std::shared_ptr<CollisionObject>>>::iterator End() { return m_Objects.End(); }

    protected:
        void UpdateQuadTreeContainer();
        void FindCollisions(std::shared_ptr<CollisionObject> objectA, std::shared_ptr<CollisionObject> objectB,
            std::vector<Collision> &collisions, std::vector<Collision> &triggers);
        void SolveCollisions(std::vector<Collision> &collisions, float deltaTime);
        void SendCollisionCallbacks(std::vector<Collision> &collisions, float deltaTime);
    protected:
        std::vector<std::shared_ptr<Solver>> m_Solvers;
        QuadTreeContainer<std::shared_ptr<CollisionObject>> m_Objects;
    }; 
}