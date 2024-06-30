#include "Collision.hpp"

namespace pe2d
{
    Collision::Collision(std::shared_ptr<CollisionObject> a, std::shared_ptr<CollisionObject> b, CollisionPoints points) :
        m_ObjectA(a), m_ObjectB(b), m_Points(points) {}

    Collision::Collision(std::shared_ptr<RigidObject> a, std::shared_ptr<RigidObject> b, CollisionPoints points) :
        m_ObjectA(a), m_ObjectB(b), m_Points(points) {}

    Collision::Collision(const Collision &other) :
        m_ObjectA(other.m_ObjectA), m_ObjectB(other.m_ObjectB), m_Points(other.m_Points) {}

    Collision::Collision(Collision &&other) noexcept:
        m_ObjectA(other.m_ObjectA), m_ObjectB(other.m_ObjectB), m_Points(other.m_Points) 
    {
        other.m_ObjectA = nullptr;
        other.m_ObjectB = nullptr;
        other.m_Points = CollisionPoints();
    };

    Collision& Collision::operator=(const Collision &other) 
    {
        if(this == &other)
        {
            return *this;
        }
        m_ObjectA = other.m_ObjectA;
        m_ObjectB = other.m_ObjectB;
        m_Points = other.m_Points;
        return *this;
    }

    Collision& Collision::operator=(Collision &&other) noexcept
    {
        if(this == &other)
        {
            return *this;
        }
        m_ObjectA = other.m_ObjectA;
        m_ObjectB = other.m_ObjectB;
        m_Points = other.m_Points;
        other.m_ObjectA = nullptr;
        other.m_ObjectB = nullptr;
        other.m_Points = CollisionPoints();
        return *this;
    }

    std::shared_ptr<CollisionObject> Collision::GetCollisionObjectA() const
    {
        return m_ObjectA;
    }

    std::shared_ptr<CollisionObject> Collision::GetCollisionObjectB() const
    {
        return m_ObjectB;
    }

    const CollisionPoints& Collision::GetPoints() const
    {
        return m_Points;
    }

    void Collision::SetCollisionObjectA(const std::shared_ptr<CollisionObject> a)
    {
        if(a == nullptr)
        {
            // Assert
        }
        m_ObjectA = a;
    }

    void Collision::SetCollisionObjectB(const std::shared_ptr<CollisionObject> b)
    {
        if(b == nullptr)
        {
            // Assert
        }
        m_ObjectB = b;
    }

    void Collision::SetPoints(const CollisionPoints &points)
    {
        m_Points = points;
    }
}