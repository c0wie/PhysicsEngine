#pragma once

#include "Collider.hpp"
#include "Algo.hpp"

namespace phy
{
    class CircleCollider : public Collider
    {
    public:
        Vector2 Center;
        float Radius;
    public:
        CircleCollider() : 
        Center(Vector2(0.0f, 0.0f)), Radius(0.0f) {};
    
        CircleCollider(const Vector2 &center, float radius) :
            Center(center), Radius(radius) {};

        CircleCollider(const CircleCollider &other) :
            Center(other.Center), Radius(other.Radius)
        {}

        CircleCollider(CircleCollider &&other) noexcept :
            Center(other.Center), Radius(other.Radius)
        {
            other.Center = Vector2(0.0f, 0.0f);
            other.Radius = 0.0f;
        }

        CircleCollider& operator=(const CircleCollider &other)
        {
            if(this == &other)
            {
                return *this;
            }
            Center = other.Center;
            Radius = other.Radius;
            return *this;
        }

        CircleCollider& operator=(CircleCollider &&other) noexcept
        {
            if(this == &other)
            {
                return *this;
            }
            Center = other.Center;
            Radius = other.Radius;
            other.Center = Vector2(0.0f, 0.0f);
            other.Radius = 0.0f;
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
            return algo::FindCircleCircleCollision(this, transform, circle, circleTransform);
        }

        CollisionPoints TestCollision(
            const Transform *transform,
            const SquareCollider *square,
            const Transform *squareTransform) const override
        {
            return algo::FindCircleSquareCollision(this, transform, square, squareTransform);
        }
    };
}