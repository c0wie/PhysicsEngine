#include "../headers/RigidObject.hpp"

namespace phy
{
    RigidObject::RigidObject() :
        CollisionObject(), m_Mass(0.0f), m_Velocity(Vector2{}), m_Force(Vector2{}), m_Gravity(Vector2{}), m_TakesGravity(false),
        m_StaticFriction(0.0f), m_DynamicFriction(0.0f), m_Restitution(0.0f) {}
    
    RigidObject::RigidObject(std::shared_ptr<Collider> collider, std::shared_ptr<Transform> transform, float trigger, 
            float mass, const Vector2 &velocity, const Vector2 &force, const Vector2 &gravity,
            bool takesGravity, float staticFriction, float dynamicFriction, float restitution) :
        CollisionObject(collider, transform, trigger), m_Mass(mass), m_Velocity(velocity), m_Force(force), m_Gravity(gravity),
        m_TakesGravity(takesGravity), m_StaticFriction(staticFriction), m_DynamicFriction(dynamicFriction), m_Restitution(restitution) {}
    
    RigidObject::RigidObject(const RigidObject &other):
        CollisionObject(other), m_Mass(other.m_Mass), m_Velocity(other.m_Velocity), m_Force(other.m_Force), m_Gravity(other.m_Gravity),
        m_TakesGravity(other.m_TakesGravity), m_StaticFriction(other.m_StaticFriction), m_DynamicFriction(other.m_DynamicFriction),
        m_Restitution(other.m_Restitution) {}

    RigidObject::RigidObject(RigidObject &&other) :
        CollisionObject(std::move(other)), m_Mass(other.m_Mass), m_Velocity(std::move(other.m_Velocity)), m_Force(std::move(other.m_Force)),
        m_Gravity(std::move(other.m_Gravity)), m_TakesGravity(other.m_TakesGravity), m_StaticFriction(other.m_StaticFriction),
        m_DynamicFriction(other.m_DynamicFriction), m_Restitution(other.m_Restitution) 
    {
        other.m_Collider = nullptr;
        other.m_Transform = nullptr;
        other.m_IsTrigger = false;
        other.m_Mass = 0.0f;
        other.m_Velocity = Vector2{};
        other.m_Force = Vector2{};
        other.m_Gravity = Vector2{};
        other.m_TakesGravity = false;
        other.m_StaticFriction = 0.0f;
        other.m_DynamicFriction = 0.0f;
        other.m_Restitution = 0.0f;
    }

    RigidObject& RigidObject::operator=(const RigidObject &other) 
    {
        if(this == &other)
        {
            return *this;
        }
        m_Collider = other.m_Collider;
        m_Transform = other.m_Transform;
        m_IsTrigger = other.m_IsTrigger;
        m_Mass = other.m_Mass;
        m_Velocity = other.m_Velocity;
        m_Force = other.m_Force;
        m_Gravity = other.m_Gravity;
        m_TakesGravity = other.m_TakesGravity;
        m_StaticFriction = other.m_StaticFriction;
        m_DynamicFriction = other.m_DynamicFriction;
        m_Restitution = other.m_Restitution;
        return *this;
    }

    RigidObject& RigidObject::operator=(RigidObject &&other) 
    {
        if(this == &other)
        {
            return *this;
        }
        m_Collider = other.m_Collider;
        m_Transform = other.m_Transform;
        m_IsTrigger = other.m_IsTrigger;
        m_Mass = other.m_Mass;
        m_Velocity = other.m_Velocity;
        m_Force = other.m_Force;
        m_Gravity = other.m_Gravity;
        m_TakesGravity = other.m_TakesGravity;
        m_StaticFriction = other.m_StaticFriction;
        m_DynamicFriction = other.m_DynamicFriction;
        m_Restitution = other.m_Restitution;

        other.m_Collider = nullptr;
        other.m_Transform = nullptr;
        other.m_IsTrigger = false;
        other.m_Mass = 0.0f;
        other.m_Velocity = Vector2{};
        other.m_Force = Vector2{};
        other.m_Gravity = Vector2{};
        other.m_TakesGravity = false;
        other.m_StaticFriction = 0.0f;
        other.m_DynamicFriction = 0.0f;
        other.m_Restitution = 0.0f;
        return *this;
    }

    float RigidObject::GetMass() const
    {
        return m_Mass;
    }

    Vector2 RigidObject::GetVelocity() const
    {
        return m_Velocity;
    }

    Vector2 RigidObject::GetForce() const
    {
        return m_Force;
    }

    Vector2 RigidObject::GetGravity() const
    {
        return m_Gravity;
    }

    float RigidObject::GetStaticFriction() const
    {
        return m_StaticFriction;
    }

    float RigidObject::GetDynamicFriction() const
    {
        return m_DynamicFriction;
    }

    float RigidObject::GetRestitution() const
    {
        return m_Restitution;
    }

    void RigidObject::SetMass(float mass)
    {
        m_Mass = mass;
    }

    void RigidObject::SetVelocity(const Vector2 &velocity)
    {
        m_Velocity = velocity;
    }

    void RigidObject::SetForce(const Vector2 &force)
    {
        m_Force = force;
    }

    void RigidObject::SetStaticFriction(float staticFriction)
    {
        m_StaticFriction = staticFriction;
    }

    void RigidObject::SetDynamicFriction(float dynamicDriction)
    {
        m_DynamicFriction = m_DynamicFriction;
    }

    void RigidObject::SetRestitution(float restitution)
    {
        m_Restitution = restitution;
    }
}