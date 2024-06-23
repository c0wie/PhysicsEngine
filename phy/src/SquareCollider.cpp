#include "../headers/SquareCollider.hpp"
#include "../headers/Algo.hpp"

namespace phy
{
    SquareCollider::SquareCollider() : 
        SideLength(0.0f) {};

    SquareCollider::SquareCollider(float sideLength) :
        SideLength(sideLength) {};

    SquareCollider::SquareCollider(const SquareCollider &other) :
        SideLength(other.SideLength)
    {}

    SquareCollider::SquareCollider(SquareCollider &&other) noexcept :
       SideLength(other.SideLength)
    {
        other.SideLength = 0.0f;
    }

    SquareCollider& SquareCollider::operator=(const SquareCollider &other)
    {
        if(this == &other)
        {
            return *this;
        }
        SideLength = other.SideLength;
        return *this;
    }

    SquareCollider& SquareCollider::operator=(SquareCollider &&other) noexcept
    {
        if(this == &other)
        {
            return *this;
        }
        SideLength = other.SideLength;
        other.SideLength = 0.0f;
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
}