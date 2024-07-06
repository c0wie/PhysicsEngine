#include "CollisionObject.hpp"

namespace pe2d
{
    CollisionObject::CollisionObject( const std::shared_ptr<CircleCollider> collider, const Transform &transform,
            bool trigger, std::function<void(Collision, float)> collisionCallback) :
        m_Collider(collider), m_Transform(transform), m_IsTrigger(trigger), m_OnCollision(collisionCallback) {}

    CollisionObject::CollisionObject( const std::shared_ptr<PolygonCollider> collider, const Transform &transform,
        bool trigger, std::function<void(Collision, float)> collisionCallback) :
        m_Collider(collider), m_Transform(transform), m_IsTrigger(trigger), m_OnCollision(collisionCallback) {}

    CollisionObject::CollisionObject(const CollisionObject &other) :
        m_Collider(other.m_Collider), m_Transform(other.m_Transform), m_IsTrigger(other.m_IsTrigger), m_OnCollision(other.m_OnCollision) {} 

    CollisionObject::CollisionObject (CollisionObject &&other) :
        m_Collider(other.m_Collider), m_Transform(other.m_Transform), m_IsTrigger(other.m_IsTrigger), m_OnCollision(other.m_OnCollision)
    {
        m_Collider = nullptr;
        m_Transform = Transform{};
        m_IsTrigger = false;
        m_OnCollision = nullptr;
    };

    CollisionObject& CollisionObject::operator=(const CollisionObject &other)
    {
        if(this == &other)
        {
            return *this;
        }
        
        m_Collider = other.m_Collider;
        m_Transform = other.m_Transform;
        m_IsTrigger = other.m_IsTrigger;
        m_OnCollision = other.m_OnCollision;
        return *this;
    }

    CollisionObject& CollisionObject::operator=(CollisionObject &&other)
    {
        if(this == &other)
        {
            return *this;
        }
        m_Collider = other.m_Collider;
        m_Transform = other.m_Transform;
        m_IsTrigger = other.m_IsTrigger;
        m_OnCollision = other.m_OnCollision;

        m_Collider = nullptr;
        m_Transform = Transform{};
        m_IsTrigger = false;
        m_OnCollision = nullptr;
        return *this;
    }
}