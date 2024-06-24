#include "../headers/CollisionObject.hpp"

namespace phy
{
    CollisionObject::CollisionObject() :
        m_Collider(nullptr), m_Transform(nullptr), m_IsTrigger(false) {}

    CollisionObject::CollisionObject( const std::shared_ptr<Collider> collider, const std::shared_ptr<phy::Transform> transform, bool trigger) :
        m_Collider(collider), m_Transform(transform), m_IsTrigger(trigger) {}

    CollisionObject::CollisionObject(const CollisionObject &other) :
        m_Collider(other.m_Collider), m_Transform(other.m_Transform), m_IsTrigger(other.m_IsTrigger) {} 

    CollisionObject::CollisionObject (CollisionObject &&other) noexcept :
        m_Collider(other.m_Collider), m_Transform(other.m_Transform), m_IsTrigger(other.m_IsTrigger)
    {
        m_Collider = nullptr;
        m_Transform = nullptr;
        m_IsTrigger = false;
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
        return *this;
    }

    CollisionObject& CollisionObject::operator=(CollisionObject &&other) noexcept
    {
        if(this == &other)
        {
            return *this;
        }
        m_Collider = other.m_Collider;
        m_Transform = other.m_Transform;
        m_IsTrigger = other.m_IsTrigger;

        m_Collider = nullptr;
        m_Transform = nullptr;
        m_IsTrigger = false;
        return *this;
    }

    const std::shared_ptr<Collider> CollisionObject::GetCollider() const
    {
        return m_Collider;
    }

    const std::shared_ptr<Transform> CollisionObject::GetTransform() const
    {
        return m_Transform;
    }

    bool CollisionObject::GetTrigger() const
    {
        return m_IsTrigger;
    }

    void CollisionObject::SetCollider(const std::shared_ptr<Collider> collider)
    {
        m_Collider = collider;
    }

    void CollisionObject::SetTransform(const std::shared_ptr<Transform> transform)
    {
        m_Transform = transform;
    }

    void CollisionObject::SetTrigger(bool trigger)
    {
        m_IsTrigger = trigger;
    }
}