#pragma once

#include "Collider.hpp"

namespace phy
{
    class SquareCollider : public Collider
    {
    public:
        float SideLength;
    public:
        SquareCollider();
        SquareCollider(float sideLength);
        SquareCollider(const SquareCollider &other);
        SquareCollider(SquareCollider &&other) noexcept;
        SquareCollider& operator=(const SquareCollider &other);
        SquareCollider& operator=(SquareCollider &&other) noexcept;

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