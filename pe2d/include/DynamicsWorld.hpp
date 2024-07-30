#pragma once

#include "CollisionWorld.hpp"

namespace pe2d
{
    // class that run all collision checks, simulate dynamics and own all items 
    // you call it to use physics engine
    class DynamicsWorld : public CollisionWorld
    {
    public:
        DynamicsWorld() = default;
        DynamicsWorld(const CollisionWorld &other) = delete;
        DynamicsWorld(DynamicsWorld &&other) = delete;
        DynamicsWorld operator=(const CollisionWorld &other) = delete;
        DynamicsWorld operator=(CollisionWorld &&other) = delete;
        ~DynamicsWorld() = default;
    public:
        void Step(float deltaTime);
    private:
        void ApplyGravity();
        void MoveObjects(float deltaTime);
    private:
        static constexpr float m_GRAVITY = 98.1f;
        static constexpr float m_MAX_VELOCITY = 1000.0f;
    };
}