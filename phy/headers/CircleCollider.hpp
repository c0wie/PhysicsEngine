#pragma once

#include "Collider.hpp"
#include "Algo.hpp"

namespace phy
{
    class CircleCollider : public Collider
    {
    public:
        float Radius;
    public:
        CircleCollider();
        CircleCollider(float radius);
        CircleCollider(const CircleCollider &other);
        CircleCollider(CircleCollider &&other) noexcept;
        CircleCollider& operator=(const CircleCollider &other);
        CircleCollider& operator=(CircleCollider &&other) noexcept;

        CollisionPoints TestCollision(
            const Transform *transform,
            const Collider *collider,
            const Transform *colliderTransform) const override;
        
        CollisionPoints TestCollision(
            const Transform *transform,
            const CircleCollider *circle,
            const Transform *circleTransform) const override;

        CollisionPoints TestCollision(
            const Transform *transform,
            const SquareCollider *square,
            const Transform *squareTransform) const override;
        
        void Draw(sf::RenderWindow &window, const Transform *transform) const override;
    };
}