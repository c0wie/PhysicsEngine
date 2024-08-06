#include "CollisionObject.hpp"
#include "Collision.hpp"
#include "Algo.hpp"

namespace pe2d
{
    CollisionObject::CollisionObject(unsigned int ID, std::shared_ptr<Collider> collider, Transform transform, bool isTrigger,
        std::function<void(Collision, float)> collisionCallback) :
        m_ID(ID), m_Collider(collider), m_Transform(transform), m_IsTrigger(isTrigger), m_IsRigid(false), m_OnCollision(collisionCallback) 
    {
        if(!collider)
        {
            ASSERT("Unvalid collider");
        }
    }

    CollisionObject::CollisionObject(const CollisionObject &other) :
        m_ID(other.m_ID), m_Collider(other.m_Collider), m_Transform(other.m_Transform),
        m_IsTrigger(other.m_IsTrigger), m_IsRigid(false), m_OnCollision(other.m_OnCollision) 
    {} 

    CollisionObject::CollisionObject (CollisionObject &&other) :
        m_ID(other.m_ID), m_Collider(other.m_Collider), 
        m_Transform(other.m_Transform), m_IsTrigger(other.m_IsTrigger), 
        m_IsRigid(false), m_OnCollision(other.m_OnCollision)
    {
        other.m_ID = 0U;
        other.m_Collider = nullptr;
        other.m_Transform = Transform{};
        other.m_IsTrigger = false;
        other.m_IsRigid = false;
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
        m_IsRigid = other.m_IsRigid;
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
        m_IsRigid = other.m_IsRigid;
        m_OnCollision = other.m_OnCollision;

        other.m_ID = 0U;
        other.m_Collider = nullptr;
        other.m_Transform = Transform();
        other.m_IsTrigger = false;
        other.m_IsRigid = false;
        other.m_OnCollision = nullptr;
        return *this;
    }

    // returns non rotated smallest rectangle which contains object 
    std::array<Vector2, 4> CollisionObject::GetBounadingBox() const
    {
        std::shared_ptr<CircleCollider> circleCollider = std::dynamic_pointer_cast<CircleCollider>( m_Collider );
        std::shared_ptr<BoxCollider> boxCollider = std::dynamic_pointer_cast<BoxCollider>( m_Collider );

        if(circleCollider)
        {
            const float diameter = circleCollider->GetRadius() * 2;
            return algo::GetBoxVertices(Vector2(diameter, diameter), m_Transform);
        }
        const std::array<Vector2, 4> vertices = algo::GetBoxVertices(boxCollider->GetSize(), m_Transform);

        Vector2 topLeftCorner = Vector2(algo::INF, algo::INF);
        Vector2 botRightCorner = Vector2(algo::MIN, algo::MIN);
        for(const auto vertex : vertices)
        {
            topLeftCorner.x = std::min(topLeftCorner.x, vertex.x);
            topLeftCorner.y = std::min(topLeftCorner.y, vertex.y);
            botRightCorner.x = std::max(botRightCorner.x, vertex.x);    
            botRightCorner.y = std::max(botRightCorner.y, vertex.y);
        }
        const Vector2 topRightCorner = Vector2(botRightCorner.x, topLeftCorner.y);
        const Vector2 botLeftCorner = Vector2(topLeftCorner.x, botRightCorner.y);
        return std::array<Vector2, 4>{topLeftCorner, topRightCorner, botRightCorner, botLeftCorner};
    }

    void CollisionObject::OnCollision(Collision &collision, float deltaTime) const
    {
        if(m_OnCollision)
        {
            m_OnCollision(collision, deltaTime);
        }
    }
}