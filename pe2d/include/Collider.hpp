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
        constexpr ConvexShapeCollider(Vector2 *vertecies, unsigned int verteciesCount, const Vector2 &center) :
            m_Vertecies(nullptr),
            m_VerteciesCount(verteciesCount)
        {
            m_Vertecies = new Vector2[m_VerteciesCount];
            for(unsigned int i = 0; i < m_VerteciesCount; i++)
            {
                m_Vertecies[i] = vertecies[i] - center;
            }
            IsConvex(center);
        }
        ConvexShapeCollider(const ConvexShapeCollider &other) = default;
        constexpr ConvexShapeCollider(ConvexShapeCollider &&other) noexcept :
            m_VerteciesCount( other.m_VerteciesCount ),
            m_Vertecies(other.m_Vertecies)

        {
            m_VerteciesCount = 0;
            m_Vertecies = nullptr;
        }
        ConvexShapeCollider& operator=(const ConvexShapeCollider &other) = default;
        constexpr ConvexShapeCollider& operator=(ConvexShapeCollider &&other) noexcept
        
        {
            if(this == &other)
            {
                return *this;
            }
            m_VerteciesCount = 0;
            m_Vertecies = nullptr;
            return *this;
        }
        ~ConvexShapeCollider() { delete[] m_Vertecies; }
    public:
        CollisionPoints TestCollision(const Transform &transform, const Collider *collider, const Transform &colliderTransform) const override final;
        CollisionPoints TestCollision(const Transform &transform, const CircleCollider *circle, const Transform &circleTransform) const override final;
        CollisionPoints TestCollision(const Transform &transform, const ConvexShapeCollider *convexShape, const Transform &convexShapeTransform) const override final;
        CollisionPoints TestCollision(const Transform &transform, const BoxCollider *box, const Transform &boxTransform) const override final;

        constexpr unsigned int GetVerteciesCount() const { return m_VerteciesCount; }
        constexpr Vector2*  GetVertecies() const { return m_Vertecies; }
        constexpr void SetVertex(unsigned int index, const Vector2 &vertex, const Vector2 &center)
        {
            if(index < m_VerteciesCount)
            {
                m_Vertecies[index] = vertex - center; 
                if(!IsConvex(center))
                {
                    ASSERT("SHAPE ISN'T CONVEX NOW");
                }
            }
        }
    private:
        // this shit doesn't work at all
        bool IsConvex(const Vector2 &center) const
        {
            bool isConvex = true;
            bool gotPositive = false;
            bool gotNegative = false;

            for(unsigned int i = 0; i < m_VerteciesCount; i++)
            {
                const Vector2 p0 = (m_Vertecies[i] + center);
                const Vector2 p1 = (m_Vertecies[(i + 1) % m_VerteciesCount] + center);
                const Vector2 p2 = (m_Vertecies[(i + 2) % m_VerteciesCount] + center);
                const Vector2 d1 = p1 - p0;
                const Vector2 d2 = p2 - p1;

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
            }
            return isConvex;
        }
    private:
        unsigned int m_VerteciesCount;
        Vector2 *m_Vertecies;
    };
}