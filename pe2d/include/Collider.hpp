#pragma once

#include <memory>
#include "CollisionPoints.hpp"
#include "Transform.hpp"


namespace pe2d
{
    class CircleCollider;
    class BoxCollider;
    
    class Collider 
    {
    public:
        virtual CollisionPoints TestCollision(const Transform &transform, const Collider *collider, const Transform &colliderTransform) const = 0;
        virtual CollisionPoints TestCollision(const Transform &transform, const CircleCollider *circle, const Transform &circleTransform) const = 0;
        virtual CollisionPoints TestCollision(const Transform &transform, const BoxCollider *box, const Transform &boxTransform) const = 0;
    };
    
    class CircleCollider final : public Collider
    {
    public:
        CircleCollider() = delete;
        constexpr CircleCollider(float radius) :
            m_Radius(radius)
        {
            if(radius <= 0.0f)
            {
                ASSERT("Radius of circle has to be greater than 0");
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
        CollisionPoints TestCollision(const Transform &transform, const Collider *collider, const Transform &colliderTransform) const override final;
        CollisionPoints TestCollision(const Transform &transform, const CircleCollider *circle, const Transform &circleTransform) const override final;
        CollisionPoints TestCollision(const Transform &transform, const BoxCollider *box, const Transform &boxTransform) const override final;
        
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

    class BoxCollider final : public Collider
    {
    public:
        BoxCollider() = delete;
        constexpr BoxCollider(const Vector2 &size) : 
            m_Size(size) 
        {
            if(size.x <= 0.0f && size.y <= 0.0f)
            {
                ASSERT("Side of box has to be greater than 0");
            }
        }
        BoxCollider(const BoxCollider &other) = default;
        constexpr BoxCollider(BoxCollider &&other) :
            m_Size(other.m_Size)
        {
            other.m_Size = Vector2{};
        }
        BoxCollider& operator=(const BoxCollider &other) = default;
        constexpr BoxCollider& operator=(BoxCollider &&other)
        {
            if(this == &other)
            {
                return *this;
            }
            m_Size = other.m_Size;
            other.m_Size = Vector2{};
            return *this;
        }
    public:
        CollisionPoints TestCollision(const Transform &transform, const Collider *collider, const Transform &colliderTransform) const override final;
        CollisionPoints TestCollision(const Transform &transform, const CircleCollider *circle, const Transform &circleTransform) const override final;
        CollisionPoints TestCollision(const Transform &transform, const BoxCollider *box, const Transform &boxTransform) const override final;
        
        constexpr Vector2 GetSize() const { return m_Size; }
        constexpr void SetSize(const Vector2 &size) 
        {
            if(size.x > 0.0f && size.y > 0.0f)
            {
                m_Size = size;
            }
        }
    private:
        Vector2 m_Size;
    };
}