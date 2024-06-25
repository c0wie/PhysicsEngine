#pragma once

#include "Collider.hpp"

namespace phy
{
    class SquareCollider : public Collider
    {
    private:
        float m_SideLength;
    public:
        SquareCollider() = delete;
        SquareCollider(float sideLength);
        SquareCollider(const SquareCollider &other) = default;
        SquareCollider(SquareCollider &&other) noexcept;
        SquareCollider& operator=(const SquareCollider &other) = default;
        SquareCollider& operator=(SquareCollider &&other) noexcept;

        CollisionPoints TestCollision(
            const Transform *transform,
            const Collider *collider,
            const Transform *colliderTransform) const override final;

        CollisionPoints TestCollision(
            const Transform *transform,
            const CircleCollider *circle,
            const Transform *circleTransform) const override final;

        CollisionPoints TestCollision(
            const Transform *transform,
            const SquareCollider *square,
            const Transform *squareTransform) const override final;

        float GetSideLength() const;
        void SetSideLength(float sideLength);
    
        void Draw(sf::RenderWindow &window, const Transform *transform) const override final;
    };
}