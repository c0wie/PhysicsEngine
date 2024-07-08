#pragma once

#include <memory>
#include "CollisionPoints.hpp"
#include "Transform.hpp"


namespace pe2d
{
    class ConvexShapeCollider;
    class CircleCollider;
    class BoxCollider;
    
    class Collider 
    {
    public:
        virtual CollisionPoints TestCollision(const Transform &transform, const Collider *collider, const Transform &colliderTransform) const = 0;
        virtual CollisionPoints TestCollision(const Transform &transform, const CircleCollider *circle, const Transform &circleTransform) const = 0;
        virtual CollisionPoints TestCollision(const Transform &transform, const ConvexShapeCollider *convexShape, const Transform &convexShapeTransform) const = 0;
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
        CollisionPoints TestCollision(const Transform &transform, const Collider *collider, const Transform &colliderTransform) const override final;
        CollisionPoints TestCollision(const Transform &transform, const CircleCollider *circle, const Transform &circleTransform) const override final;
        CollisionPoints TestCollision(const Transform &transform, const ConvexShapeCollider *convexShape, const Transform &convexShapeTransform) const override final;
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
                ASSERT("SIZE CANNOT BE NEGATIVE");
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
        CollisionPoints TestCollision(const Transform &transform, const ConvexShapeCollider *convexShape, const Transform &convexShapeTransform) const override final;
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
    class ConvexShapeCollider final : public Collider
    {
    public:
        ConvexShapeCollider() = delete;
        constexpr ConvexShapeCollider(unsigned int verteciesCount, float sideLength) :
            m_SideLength(sideLength),
            m_VerteciesCount(verteciesCount),
            m_Vertecies(nullptr)
        {
            if(m_SideLength <= 0.0f)
            {
                ASSERT("SideLenth must be grater than 0");
            }
            if(m_VerteciesCount < 3)
            {
                ASSERT("POLYGON MUST HAVE AT LEAST 3 VERTECIES");
            }
            m_Vertecies = new Vector2[verteciesCount];
        }
        ConvexShapeCollider(const ConvexShapeCollider &other) = default;
        constexpr ConvexShapeCollider(ConvexShapeCollider &&other) noexcept :
            m_SideLength( other.m_SideLength ),
            m_VerteciesCount( other.m_VerteciesCount ),
            m_Vertecies(other.m_Vertecies)

        {
            if(m_VerteciesCount != other.m_VerteciesCount)
            {
                ASSERT("CAN'T CONSTRUCT ConvexShape SHAPE FROM OTHER ConvexShape SHAPE WITH DIFFRENT NUMBER OF VERTECIES");
            }
            other.m_SideLength = 0.0f;
            m_VerteciesCount = 0.0f;
            m_Vertecies = nullptr;
        }
        ConvexShapeCollider& operator=(const ConvexShapeCollider &other) = default;
        constexpr ConvexShapeCollider& operator=(ConvexShapeCollider &&other) noexcept
        
        {
            if(this == &other)
            {
                return *this;
            }
            m_SideLength = other.m_SideLength;
            other.m_SideLength = 0.0f;
            return *this;
        }
        ~ConvexShapeCollider() { delete[] m_Vertecies; }
    public:
        CollisionPoints TestCollision(const Transform &transform, const Collider *collider, const Transform &colliderTransform) const override final;
        CollisionPoints TestCollision(const Transform &transform, const CircleCollider *circle, const Transform &circleTransform) const override final;
        CollisionPoints TestCollision(const Transform &transform, const ConvexShapeCollider *convexShape, const Transform &convexShapeTransform) const override final;
        CollisionPoints TestCollision(const Transform &transform, const BoxCollider *box, const Transform &boxTransform) const override final;

        constexpr float GetSideLength() const { return m_SideLength; }
        constexpr unsigned int GetVerteciesCount() const { return m_VerteciesCount; }
        constexpr Vector2* GetVertecies() const {return m_Vertecies; }
        constexpr void SetSideLength(float sideLength)
        {
            if(sideLength <= 0.0f)
            {
                ASSERT("SideLenth must be grater than 0");
            }
            m_SideLength = sideLength;
        }
        constexpr void SetVerteciesCount(unsigned int verteciesCount) { m_VerteciesCount = verteciesCount; }
        constexpr void SetVertex(unsigned int index, const Vector2 &vertex, const Vector2 &center)
        {
            if(index < m_VerteciesCount)
            {
                m_Vertecies[index] = vertex - center; 
                if(!IsConvex())
                {
                    ASSERT("SHAPE ISN'T CONVEX NOW");
                }
            }
        }
    private:
        bool IsConvex() const
        {
            bool isConvex = true;
            bool gotPositive = false;
            bool gotNegative = false;

            for(unsigned int i = 0; i < m_VerteciesCount; i++)
            {
                const Vector2 *const p0 = &m_Vertecies[i];
                const Vector2 *const p1 = &m_Vertecies[(i + 1) % m_VerteciesCount];
                const Vector2 *const p2 = &m_Vertecies[(i + 2) % m_VerteciesCount];
                if(!p0 || !p1 || !p2)
                {
                    return true;
                }
                const Vector2 d1 = *p1 - *p0;
                const Vector2 d2 = *p2 - *p1;

                const float cross = d1.x * d2.y - d1.y * d2.x;
                if(cross < 0.0f)
                {
                    gotNegative= true;
                }
                else if(cross > 0.0f)
                {
                    gotPositive = true;
                }
                if(gotNegative && gotPositive)
                {
                    isConvex = false;
                    break;
                }
                return isConvex;
            }
        }
    private:
        float m_SideLength;
        unsigned int m_VerteciesCount;
        Vector2 *m_Vertecies;
    };
}