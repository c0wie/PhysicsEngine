#pragma once

#include <memory>
#include "CollisionPoints.hpp"
#include "Transform.hpp"


namespace pe2d
{
    class SquareCollider;
    class CircleCollider;
    
    class Collider 
    {
    public:
        virtual CollisionPoints TestCollision(
            const Transform &transform,
            const Collider *collider,
            const Transform &colliderTransform) const = 0;
        
        virtual CollisionPoints TestCollision(
            const Transform &transform,
            const CircleCollider *circle,
            const Transform &circleTransform) const = 0;
        
        virtual CollisionPoints TestCollision(
            const Transform &transform,
            const SquareCollider *square,
            const Transform &squareTransform) const = 0;
        
    };
    
    class CircleCollider : public Collider
    {
    public:
        CircleCollider() = delete;
        CircleCollider(float radius);
        CircleCollider(const CircleCollider &other) = default;
        CircleCollider(CircleCollider &&other) noexcept;
        CircleCollider& operator=(const CircleCollider &other) = default;
        CircleCollider& operator=(CircleCollider &&other) noexcept;
    public:
        CollisionPoints TestCollision(
            const Transform &transform,
            const Collider *collider,
            const Transform &colliderTransform) const override final;
        
        CollisionPoints TestCollision(
            const Transform &transform,
            const CircleCollider *circle,
            const Transform &circleTransform) const override final;

        CollisionPoints TestCollision(
            const Transform &transform,
            const SquareCollider *square,
            const Transform &squareTransform) const override final;
        
        float GetRadius() const;
        void SetRadius(float radius);
    private:
        float m_Radius;
    };

    class SquareCollider : public Collider
    {
    public:
        SquareCollider() = delete;
        SquareCollider(float sideLength);
        SquareCollider(const SquareCollider &other) = default;
        SquareCollider(SquareCollider &&other) noexcept;
        SquareCollider& operator=(const SquareCollider &other) = default;
        SquareCollider& operator=(SquareCollider &&other) noexcept;
    public:
        CollisionPoints TestCollision(
            const Transform &transform,
            const Collider *collider,
            const Transform &colliderTransform) const override final;

        CollisionPoints TestCollision(
            const Transform &transform,
            const CircleCollider *circle,
            const Transform &circleTransform) const override final;

        CollisionPoints TestCollision(
            const Transform &transform,
            const SquareCollider *square,
            const Transform &squareTransform) const override final;

        float GetSideLength() const;
        void SetSideLength(float sideLength);
    private:
        float m_SideLength;
    };
}