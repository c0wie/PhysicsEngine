#include "Collider.hpp"
#include "Algo.hpp"
namespace pe2d
{
    CircleCollider::CircleCollider(float radius) :
        m_Radius(radius) 
    {   
        if(radius <= 0.0f)
        {
            ASSERT("Radius must be grater than 0");
        }
    };

    CircleCollider::CircleCollider(CircleCollider &&other) noexcept :
        m_Radius(other.m_Radius)
    {
        other.m_Radius = 0.0f;
    }

    CircleCollider& CircleCollider::operator=(CircleCollider &&other) noexcept
    {
        if(this == &other)
        {
            return *this;
        }
        m_Radius = other.m_Radius;
        other.m_Radius = 0.0f;
        return *this;
    }

    CollisionPoints CircleCollider::TestCollision(
        const Transform &transform,
        const Collider *collider,
        const Transform &colliderTransform) const 
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }
    
    CollisionPoints CircleCollider::TestCollision(
        const Transform &transform,
        const CircleCollider *circle,
        const Transform &circleTransform) const 
    {
        return Algo::FindCircleCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints CircleCollider::TestCollision(
        const Transform &transform,
        const SquareCollider *square,
        const Transform &squareTransform) const 
    {
        return Algo::FindCircleSquareCollision(this, transform, square, squareTransform);
    }

    float CircleCollider::GetRadius() const
    {
        return m_Radius;
    }

    void  CircleCollider::SetRadius(float radius)
    {
        if(radius <= 0.0f)
        {
            ASSERT("Radius must be grater than 0");
        }
        m_Radius = radius;
    }
}