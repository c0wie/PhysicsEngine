#include "CollisionObject.hpp"
#include "Collision.hpp"

namespace pe2d
{
    CollisionObject::CollisionObject(unsigned int ID, std::shared_ptr<Collider> collider, Transform transform, bool isTrigger, bool isMovable,
        std::function<void(Collision, float)> collisionCallback) :
        m_ID(ID), m_Collider(collider), m_Transform(transform), m_IsTrigger(isTrigger), m_IsMovable(isMovable), m_OnCollision(collisionCallback) 
    {
        if(!collider)
        {
            ASSERT("Unvalid collider");
        }
    }

    CollisionObject::CollisionObject(const CollisionObject &other) :
        m_ID(other.m_ID), m_Collider(other.m_Collider), m_Transform(other.m_Transform), m_IsTrigger(other.m_IsTrigger),
        m_IsMovable(other.m_IsMovable), m_OnCollision(other.m_OnCollision) 
    {} 

    CollisionObject::CollisionObject (CollisionObject &&other) :
        m_ID(other.m_ID), m_Collider(other.m_Collider), 
        m_Transform(other.m_Transform), m_IsTrigger(other.m_IsTrigger),
        m_IsMovable(other.m_IsMovable), m_OnCollision(other.m_OnCollision)
    {
        other.m_ID = 0U;
        other.m_Collider = nullptr;
        other.m_Transform = Transform{};
        other.m_IsTrigger = false;
        other.m_IsMovable = false;
        other.m_OnCollision = nullptr;
    };

    CollisionObject& CollisionObject::operator=(const CollisionObject &other)
    {
        if(this == &other)
        {
            return *this;
        }
        
        m_ID = other.m_ID;
        m_Collider = other.m_Collider;
        m_Transform = other.m_Transform;
        m_IsTrigger = other.m_IsTrigger;
        m_IsMovable = other.m_IsMovable;
        m_OnCollision = other.m_OnCollision;
        return *this;
    }

    CollisionObject& CollisionObject::operator=(CollisionObject &&other)
    {
        if(this == &other)
        {
            return *this;
        }
        m_ID = other.m_ID;
        m_Collider = other.m_Collider;
        m_Transform = other.m_Transform;
        m_IsTrigger = other.m_IsTrigger;
        m_IsMovable = other.m_IsMovable;
        m_OnCollision = other.m_OnCollision;

        other.m_ID = 0U;
        other.m_Collider = nullptr;
        other.m_Transform = Transform();
        other.m_IsTrigger = false;
        other.m_IsMovable = false;
        other.m_OnCollision = nullptr;
        return *this;
    }

    Vector2 CollisionObject::GetBounadingBox() const
    {
        std::shared_ptr<CircleCollider> circleCollider = std::dynamic_pointer_cast<CircleCollider>( m_Collider );
        std::shared_ptr<BoxCollider> boxCollider = std::dynamic_pointer_cast<BoxCollider>( m_Collider );
        if(!circleCollider && !boxCollider)
        {
            ASSERT("Unvalid type of collider");
        }

        if(circleCollider)
        {
            const float radius = circleCollider->GetRadius();
            return Vector2(radius, radius);
        }
        
        return boxCollider->GetSize();
    }

    void CollisionObject::OnCollision(Collision &collision, float deltaTime) const
    {
        if(m_OnCollision)
        {
            m_OnCollision(collision, deltaTime);
        }
    }
}