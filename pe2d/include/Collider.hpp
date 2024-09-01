#pragma once

#include <memory>
#include "CollisionPoints.hpp"
#include "Transform.hpp"


namespace pe2d
{
    class CircleCollider;
    class BoxCollider;
    
    // Pure virtual class wraper around algo namespace.
    class Collider
    {
    public:
        virtual CollisionPoints TestCollision(Transform transform, std::shared_ptr<Collider> collider, Transform colliderTransform) = 0;
        virtual CollisionPoints TestCollision(Transform transform, std::shared_ptr<CircleCollider> circle, Transform circleTransform) = 0;
        virtual CollisionPoints TestCollision(Transform transform, std::shared_ptr< BoxCollider> box, Transform boxTransform) = 0;
    };
    
    // Wraper class arround algo namespace that allows to retrive information about circle collisions.
    class CircleCollider final : public Collider, public std::enable_shared_from_this<CircleCollider>
    {
    public:
        constexpr CircleCollider(float radius)
        {
            SetRadius(radius);
        }
    public:
        CollisionPoints TestCollision(Transform transform, std::shared_ptr<Collider> collider, Transform colliderTransform) override final;
        CollisionPoints TestCollision(Transform transform, std::shared_ptr<CircleCollider> circle, Transform circleTransform) override final;
        CollisionPoints TestCollision(Transform transform, std::shared_ptr< BoxCollider> box, Transform boxTransform) override final;
        
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
        float m_Radius{10.0f};
    };

    // Wraper class arround algo namespace that allows to retrive information about box collisions.
    class BoxCollider final : public Collider, public std::enable_shared_from_this<BoxCollider>
    {
    public:
        explicit constexpr BoxCollider(Vector2 size)
        {
            SetSize(size);
        }
        BoxCollider(float x, float y)
        {
            SetSize({x, y});
        }
    public:
        CollisionPoints TestCollision(Transform transform, std::shared_ptr<Collider> collider, Transform colliderTransform) override final;
        CollisionPoints TestCollision(Transform transform, std::shared_ptr<CircleCollider> circle, Transform circleTransform) override final;
        CollisionPoints TestCollision(Transform transform, std::shared_ptr< BoxCollider> box, Transform boxTransform) override final;
        
        constexpr Vector2 GetSize() const { return m_Size; }
        constexpr void SetSize(const Vector2 &size) 
        {
            if(size.x <= 0.0f && size.y <= 0.0f)
            {
                ASSERT("Side of box has to be greater than 0");
            }
            m_Size = size;
        }
    private:
        Vector2 m_Size{10.0f, 10.0f};
    };
}