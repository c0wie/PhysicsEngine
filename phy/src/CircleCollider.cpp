#include "../headers/CircleCollider.hpp"
#include "../headers/Algo.hpp"

namespace phy
{
    CircleCollider::CircleCollider() : 
    Center(Vector2(0.0f, 0.0f)), Radius(0.0f) {};

    CircleCollider::CircleCollider(const Vector2 &center, float radius) :
        Center(center), Radius(radius) {};

    CircleCollider::CircleCollider(const CircleCollider &other) :
        Center(other.Center), Radius(other.Radius)
    {}

    CircleCollider::CircleCollider(CircleCollider &&other) noexcept :
        Center(other.Center), Radius(other.Radius)
    {
        other.Center = Vector2(0.0f, 0.0f);
        other.Radius = 0.0f;
    }

    CircleCollider& CircleCollider::operator=(const CircleCollider &other)
    {
        if(this == &other)
        {
            return *this;
        }
        Center = other.Center;
        Radius = other.Radius;
        return *this;
    }

    CircleCollider& CircleCollider::operator=(CircleCollider &&other) noexcept
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

    CollisionPoints CircleCollider::TestCollision(
        const Transform *transform,
        const Collider *collider,
        const Transform *colliderTransform) const 
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }
    
    CollisionPoints CircleCollider::TestCollision(
        const Transform *transform,
        const CircleCollider *circle,
        const Transform *circleTransform) const 
    {
        return algo::FindCircleCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints CircleCollider::TestCollision(
        const Transform *transform,
        const SquareCollider *square,
        const Transform *squareTransform) const 
    {
        return algo::FindCircleSquareCollision(this, transform, square, squareTransform);
    }
}