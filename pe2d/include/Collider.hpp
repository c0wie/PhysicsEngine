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
        constexpr CircleCollider(float radius) :
            m_Radius(radius)
        {
            if(radius <= 0.0f)
            {
                ASSERT("Radius must be grater than 0");
            }
        }
        CircleCollider(const CircleCollider &other) = default;
        constexpr CircleCollider(CircleCollider &&other) noexcept :
            m_Radius( other.m_Radius )
        {
            other.m_Radius = 0.0f;
        }
        CircleCollider& operator=(const CircleCollider &other) = default;
        constexpr CircleCollider& operator=(CircleCollider &&other) noexcept
        {
            if(this == &other)
            {
                return *this;
            }
            m_Radius = other.m_Radius;
            other.m_Radius = 0.0f;
            return *this;
        }
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
        
        constexpr float GetRadius() const { return m_Radius; }
        constexpr void SetRadius(float radius)
        {
            if(radius <= 0.0f)
            {
                ASSERT("Radius must be grater than 0");
            }
            m_Radius = radius;
        }
    private:
        float m_Radius;
    };

    class SquareCollider : public Collider
    {
    public:
        SquareCollider() = delete;
        constexpr SquareCollider(unsigned int verteciesCount, float sideLength) :
            m_SideLength(sideLength),
            m_VerteciesCount(verteciesCount)
        {
            if(m_SideLength <= 0.0f)
            {
                ASSERT("SideLenth must be grater than 0");
            }
        }
        // constructs a square
        constexpr SquareCollider(float sideLength) :
            m_SideLength(sideLength),
            m_VerteciesCount(4)
        {
            if(m_SideLength <= 0.0f)
            {
                ASSERT("SideLenth must be grater than 0");
            }
        }
        SquareCollider(const SquareCollider &other) = default;
        constexpr SquareCollider(SquareCollider &&other) noexcept :
            m_SideLength( other.m_SideLength ),
            m_VerteciesCount( other.m_VerteciesCount )
        {
            other.m_SideLength = 0.0f;
        }
        SquareCollider& operator=(const SquareCollider &other) = default;
        constexpr SquareCollider& operator=(SquareCollider &&other) noexcept
        {
            if(this == &other)
            {
                return *this;
            }
            m_SideLength = other.m_SideLength;
            other.m_SideLength = 0.0f;
            return *this;
        }
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

        constexpr float GetSideLength() const { return m_SideLength; }
        constexpr unsigned int GetVerteciesCount() const { return m_VerteciesCount; }
        constexpr void SetSideLength(float sideLength)
        {
            if(sideLength <= 0.0f)
            {
                ASSERT("SideLenth must be grater than 0");
            }
            m_SideLength = sideLength;
        }
        constexpr void SetVerteciesCount(unsigned int verteciesCount) { m_VerteciesCount = verteciesCount; }
    private:
        float m_SideLength;
        unsigned int m_VerteciesCount;
    };
}