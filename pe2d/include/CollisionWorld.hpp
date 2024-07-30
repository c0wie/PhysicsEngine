#pragma once

#include <unordered_map>
#include <memory>
#include <algorithm>

#include "Collision.hpp"
#include "Solver.hpp"
#include "Grid.hpp"

namespace pe2d
{
    // class that run all collision checks and own all items 
    // you call it to use physics engine
    class CollisionWorld
    {
    public:
        CollisionWorld();
        CollisionWorld(const CollisionWorld &other) = delete;
        CollisionWorld(CollisionWorld &&other) = delete;
        CollisionWorld operator = (const CollisionWorld &other) = delete;
        CollisionWorld operator = (CollisionWorld &&other) = delete;
        virtual ~CollisionWorld() = default;
    public:
        void AddObject(std::shared_ptr<CollisionObject> object);
        void RemoveObject(size_t ID) { m_Objects.erase(ID); }

        void AddSolver(std::shared_ptr<Solver> &solver);
        void RemoveSolver(std::shared_ptr<Solver> &solver);
        
        void AddGrid(Vector2 topLeftCorner, Vector2 bottomRightCorner, float cellSize);
        void RemoveGrid();
        void ResizeGrid(Vector2 topLeftCorner, Vector2 bottomRightCorner, float cellSize);
        
        void ResolveCollisions(float deltaTime);
        void ClearObjects() { m_Objects.clear(); }
        
        size_t Size() const { return m_Objects.size(); }
        bool Empty() const { return m_Objects.empty(); }
        std::unordered_map<size_t, std::shared_ptr<CollisionObject>>::iterator Begin() { return m_Objects.begin(); }
        std::unordered_map<size_t, std::shared_ptr<CollisionObject>>::iterator End() { return m_Objects.end(); }
        std::unordered_map<size_t, std::shared_ptr<CollisionObject>>::const_iterator cBegin() const { return m_Objects.cbegin(); }
        std::unordered_map<size_t, std::shared_ptr<CollisionObject>>::const_iterator cEnd() const { return m_Objects.cend(); }
        std::shared_ptr<CollisionObject> At(unsigned int ID) { return m_Objects.at(ID); }

    protected:
        void FindCollisions(std::shared_ptr<CollisionObject> objectA, std::shared_ptr<CollisionObject> objectB,
            std::vector<Collision> &collisions, std::vector<Collision> &triggers);
        void SolveCollisions(std::vector<Collision> &collisions, float deltaTime);
        void SendCollisionCallbacks(std::vector<Collision> &collisions, float deltaTime);
    protected:
        std::vector<std::shared_ptr<Solver>> m_Solvers;
        std::unordered_map<size_t, std::shared_ptr<CollisionObject>> m_Objects;
        Grid m_Grid;
        bool m_IsGridOn;
    }; 
}