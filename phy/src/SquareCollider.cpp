#include "../headers/SquareCollider.hpp"
#include "../headers/Algo.hpp"

namespace phy
{
    SquareCollider::SquareCollider() : 
        Square(Vector2(0.0f, 0.0f)), Distance(0.0f) {};

    SquareCollider::SquareCollider(const Vector2 &square, float distance) :
        Square(Square), Distance(distance) {};

    SquareCollider::SquareCollider(const SquareCollider &other) :
        Square(other.Square), Distance(other.Distance)
    {}

    SquareCollider::SquareCollider(SquareCollider &&other) noexcept :
        Square(other.Square), Distance(other.Distance)
    {
        other.Square = Vector2(0.0f, 0.0f);
        other.Distance = 0.0f;
    }

    SquareCollider& SquareCollider::operator=(const SquareCollider &other)
    {
        if(this == &other)
        {
            return *this;
        }
        Square = other.Square;
        Distance = other.Distance;
        return *this;
    }

    SquareCollider& SquareCollider::operator=(SquareCollider &&other) noexcept
    {
        if(this == &other)
        {
            return *this;
        }
        Square = other.Square;
        Distance = other.Distance;
        other.Square = Vector2(0.0f, 0.0f);
        other.Distance = 0.0f;
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
        return algo::FindSquareCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints SquareCollider::TestCollision(
        const Transform *transform,
        const SquareCollider *square,
        const Transform *squareTransform) const 
    {
        return algo::FindSquareSquareCollision(this, transform, square, squareTransform);
    }
}