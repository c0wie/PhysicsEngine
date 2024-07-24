#pragma once

#include "CollisionWorld.hpp"

namespace pe2d
{
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
        void AddRigidObject(std::shared_ptr<RigidObject> obj);
        void Step(float deltaTime);
    private:
        void ApplyGravity();
        void MoveObjects(float deltaTime);
    private:
        static constexpr float m_GRAVITY = 98.1f;
        static constexpr float m_MAX_VELOCITY = 1000.0f;
    };
}