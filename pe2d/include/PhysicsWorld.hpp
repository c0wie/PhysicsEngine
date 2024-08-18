#pragma once

#include <unordered_map>
#include <memory>
#include <algorithm>

#include "Collision.hpp"
#include "Solver.hpp"
#include "Grid.hpp"
#include "Math.hpp"

namespace pe2d
{
    inline std::vector<Vector2> thingsToDraw;
    // class that run all collision checks and own all items 
    // you call it to use physics engine
    class PhysicsWorld
    {
        using iterator = std::unordered_map<size_t, RigidObject>::iterator;
        using const_iterator = std::unordered_map<size_t, RigidObject>::const_iterator;
    public:
        PhysicsWorld() : 
            m_Grid(),
            m_IsGridOn(false),
            m_Substeps(8)
        {}
        PhysicsWorld(unsigned int substeps) :
            m_Grid(),
            m_IsGridOn(false),
            m_Substeps(std::clamp(substeps, 1U, 64U))
        {}
        PhysicsWorld(const PhysicsWorld &other) = delete;
        PhysicsWorld(PhysicsWorld &&other) = delete;
        PhysicsWorld operator = (const PhysicsWorld &other) = delete;
        PhysicsWorld operator = (PhysicsWorld &&other) = delete;
    public:
        void Step(float deltaTime);
        
        void AddObject(const RigidObject &object);
        iterator RemoveObject(size_t ID);
        
        iterator RemoveObject (iterator object) { return m_Objects.erase(object); }

        iterator RemoveObjects (iterator firstObject, std::unordered_map<size_t, RigidObject>::iterator lastObject) 
        { return m_Objects.erase(firstObject, lastObject); }
             
        void ClearObjects() { m_Objects.clear(); }

        void AddSolver(std::shared_ptr<Solver> &solver);
        void RemoveSolver(std::shared_ptr<Solver> &solver);
        
        void AddGrid(Vector2 topLeftCorner, Vector2 bottomRightCorner, float cellSize);
        void RemoveGrid();
        void ResizeGrid(Vector2 topLeftCorner, Vector2 bottomRightCorner, float cellSize);
        
        void ResolveCollisions(float deltaTime);
        
        bool IsGridOn() const { return m_IsGridOn; }
        size_t Size() const { return m_Objects.size(); }
        bool Empty() const { return m_Objects.empty(); }
        iterator Begin() { return m_Objects.begin(); }
        iterator End() { return m_Objects.end(); }
        const_iterator cBegin() const { return m_Objects.cbegin(); }
        const_iterator cEnd() const { return m_Objects.cend(); }
        RigidObject& At(unsigned int ID) { return m_Objects.at(ID); }

    private:
        void FindCollisions(size_t IDA, size_t IDB, std::vector<Collision> &collisions);
        void SolveCollisions(std::vector<Collision> &collisions, float deltaTime);
        void ApplyGravity();
        void ApplyFriction(std::vector<Collision> &collisions);
        void MoveObjects(float deltaTime);
    private:
        std::vector<std::shared_ptr<Solver>> m_Solvers;
        std::unordered_map<size_t, RigidObject> m_Objects;
        Grid m_Grid;
        bool m_IsGridOn;
        unsigned int m_Substeps;
    }; 
}