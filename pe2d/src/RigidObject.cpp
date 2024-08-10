#include "RigidObject.hpp"

namespace pe2d
{
    RigidObject::RigidObject(unsigned int ID, std::shared_ptr<Collider> collider, Transform transform,
                            bool isTrigger, std::function<void(Collision, float)> collisionCallback,
                            float mass, Vector2 velocity, Vector2 force, Vector2 gravity, bool takesGravity, 
                            float staticFriction, float dynamicFriction, float restitution) :
        CollisionObject(ID, collider, transform, isTrigger, collisionCallback),
        m_Mass(mass), m_Velocity(velocity), m_Force(force), m_Gravity(gravity),
        m_TakesGravity(takesGravity), m_StaticFriction(staticFriction),
        m_DynamicFriction(dynamicFriction), m_Restitution(restitution) 
        {
            if(m_Mass <= 0.0f)
            {
                ASSERT("Mass of object has to be greater than 0");
            }
            if(m_DynamicFriction < 0.0f || m_DynamicFriction > 1.0f)
            {
                ASSERT("Value of dynamic friction has to be beetwen 0 and 1");
            }
            if(m_StaticFriction < 0.0f || m_StaticFriction > 1.0f)
            {
                ASSERT("Value of static friction have to be beetwen 0 and 1");
            }
            m_IsRigid = true;
        }
    
    RigidObject::RigidObject(const RigidObject &other):
        CollisionObject(other), m_Mass(other.m_Mass), m_Velocity(other.m_Velocity), m_Force(other.m_Force), m_Gravity(other.m_Gravity),
        m_TakesGravity(other.m_TakesGravity), m_StaticFriction(other.m_StaticFriction),
        m_DynamicFriction(other.m_DynamicFriction), m_Restitution(other.m_Restitution) {}

    RigidObject::RigidObject(RigidObject &&other) :
        CollisionObject(std::move(other)), m_Mass(other.m_Mass), m_Velocity(other.m_Velocity), m_Force(other.m_Force),
        m_Gravity(other.m_Gravity), m_TakesGravity(other.m_TakesGravity), m_StaticFriction(other.m_StaticFriction),
        m_DynamicFriction(other.m_DynamicFriction), m_Restitution(other.m_Restitution) 
    {
        other.m_Collider = nullptr;
        other.m_Transform = Transform{};
        other.m_IsTrigger = false;
        other.m_Mass = 0.0f;
        other.m_Velocity = Vector2(0.0f, 0.0f);
        other.m_Force = Vector2(0.0f, 0.0f);
        other.m_Gravity = Vector2(0.0f, 0.0f);
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
        other.m_Transform = Transform{};
        other.m_IsTrigger = false;
        other.m_Mass = 0.0f;
        other.m_Velocity = Vector2(0.0f, 0.0f);
        other.m_Force = Vector2(0.0f, 0.0f);
        other.m_Gravity = Vector2(0.0f, 0.0f);
        other.m_TakesGravity = false;
        other.m_StaticFriction = 0.0f;
        other.m_DynamicFriction = 0.0f;
        other.m_Restitution = 0.0f;
        return *this;
    }

}