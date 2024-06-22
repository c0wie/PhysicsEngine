#pragma once

#include "Collider.hpp"
#include "Algo.hpp"

namespace phy
{
    class SquareCollider : public Collider
    {
    public:
        Vector2 Square;
        float Distance;
    public:
        SquareCollider() : 
            Square(Vector2(0.0f, 0.0f)), Distance(0.0f) {};

        SquareCollider(const Vector2 &square, float distance) :
            Square(Square), Distance(distance) {};

        SquareCollider(const SquareCollider &other) :
            Square(other.Square), Distance(other.Distance)
        {}

        SquareCollider(SquareCollider &&other) noexcept :
            Square(other.Square), Distance(other.Distance)
        {
            other.Square = Vector2(0.0f, 0.0f);
            other.Distance = 0.0f;
        }

        SquareCollider& operator=(const SquareCollider &other)
        {
            if(this == &other)
            {
                return *this;
            }
            Square = other.Square;
            Distance = other.Distance;
            return *this;
        }

        SquareCollider& operator=(SquareCollider &&other) noexcept
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

        CollisionPoints TestCollision(
            const Transform *transform,
            const Collider *collider,
            const Transform *colliderTransform) const override
        {
            return collider->TestCollision(colliderTransform, this, transform);
        }

        CollisionPoints TestCollision(
            const Transform *transform,
            const CircleCollider *circle,
            const Transform *circleTransform) const override
        {
            return algo::FindSquareCircleCollision(this, transform, circle, circleTransform);
        }

        CollisionPoints TestCollision(
            const Transform *transform,
            const SquareCollider *square,
            const Transform *squareTransform) const override
        {
            
            return CollisionPoints();
        }
    };
}