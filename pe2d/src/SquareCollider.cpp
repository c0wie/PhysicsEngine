#include "Collider.hpp"
#include "Algo.hpp"
namespace pe2d
{
    SquareCollider::SquareCollider(float sideLength)
    {
        SetSideLength(sideLength);
    }

    SquareCollider::SquareCollider(SquareCollider &&other) noexcept :
       m_SideLength(other.m_SideLength)
    {
        other.m_SideLength = 0.0f;
    }

    SquareCollider& SquareCollider::operator=(SquareCollider &&other) noexcept
    {
        if(this == &other)
        {
            return *this;
        }
        m_SideLength = other.m_SideLength;
        other.m_SideLength = 0.0f;
        return *this;
    }

    CollisionPoints SquareCollider::TestCollision(
        const Transform *transform,
        const Collider *collider,
        const Transform *colliderTransform) const 
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }

    CollisionPoints SquareCollider::TestCollision(
        const Transform *transform,
        const CircleCollider *circle,
        const Transform *circleTransform) const 
    {
        return Algo::FindSquareCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints SquareCollider::TestCollision(
        const Transform *transform,
        const SquareCollider *square,
        const Transform *squareTransform) const 
    {
        return Algo::FindSquareSquareCollision(this, transform, square, squareTransform);
    }

    float SquareCollider::GetSideLength() const
    {
        return m_SideLength;
    }

    void SquareCollider::SetSideLength(float sideLength)
    {
        if(sideLength <= 0.0f)
        {
            ASSERT("SideLenth must be grater than 0");
        }
        m_SideLength = sideLength;
    }
}