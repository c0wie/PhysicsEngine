#pragma once

#include "CollisionObject.hpp"

namespace pe2d
{
    // class that holds information about object with dynamics
    class RigidObject : public CollisionObject
    {
    public:
        RigidObject(unsigned int ID, std::shared_ptr<Collider> collider, Transform transform,
                    bool isTrigger, std::function<void(Collision, float)> collisionCallback,
                    float mass, Vector2 velocity, Vector2 force, Vector2 gravity, bool takesGravity,
                    float staticFriction, float dynamicFriction, float restitution);
        RigidObject(const RigidObject &other);
        RigidObject(RigidObject &&other);
        RigidObject& operator=(const RigidObject &other);
        RigidObject& operator=(RigidObject &&other);
        RigidObject() = default;
    public:
        constexpr float GetMass() const { return m_Mass; }
        constexpr Vector2 GetVelocity() const { return m_Velocity; }
        constexpr Vector2 GetForce() const { return m_Force; }
        constexpr Vector2 GetGravity() const { return m_Gravity; }
        constexpr bool TakesGravity() const { return m_TakesGravity; }
        constexpr float GetStaticFriction() const { return m_StaticFriction; }
        constexpr float GetDynamicFriction() const { return m_DynamicFriction; }
        constexpr float GetRestitution() const { return m_Restitution; }

        constexpr void SetMass(float mass)
        {
            if(mass <= 0.0f)
            {
                ASSERT("Mass of object has to be greater than 0");
            }
            m_Mass = mass;
        }
        constexpr void SetVelocity(Vector2 velocity)
        {
            m_Velocity = velocity;
        }
        constexpr void SetForce(Vector2 force)
        {
            m_Force = force;
        }
        constexpr void SetGravity(Vector2 gravity)
        {
            if(m_TakesGravity)
            {
                ASSERT("Can't change gravity value if object get it from environment");
            }
            m_Gravity = gravity;
        }
        constexpr void SetStaticFriction(float staticFriction)
        {
            m_StaticFriction = staticFriction;
        }
        constexpr void SetDynamicFriction(float dynamicDriction)
        {
            m_DynamicFriction = dynamicDriction;
        }
        constexpr void SetRestitution(float restitution)
        {
            m_Restitution = restitution;
        }
    private:
        float m_Mass;
        Vector2 m_Velocity;
        Vector2 m_Force;
        Vector2 m_Gravity;          // Gravitional acceleration
        bool m_TakesGravity;        // If the rigidobject will take gravity from the world
        float m_StaticFriction;     // Static friction coefficient
        float m_DynamicFriction;    // Dynamic friction coefficient
        float m_Restitution;        // Elasticy of collision
    };
}