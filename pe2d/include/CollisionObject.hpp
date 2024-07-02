#pragma once

#include <memory>
#include <functional>

#include "Collider.hpp"

class Collision;
namespace pe2d
{
    class CollisionObject
    {
    public:
        CollisionObject(const std::shared_ptr<CircleCollider> collider, const Transform &transform, bool trigger);
        CollisionObject(const std::shared_ptr<SquareCollider> collider, const Transform &transform, bool trigger);
        CollisionObject(const CollisionObject &other);
        CollisionObject(CollisionObject &&other) noexcept;
        CollisionObject& operator=(const CollisionObject &other);
        CollisionObject& operator=(CollisionObject &&other) noexcept;
        virtual ~CollisionObject() = default;
    public:
        const std::shared_ptr<Collider> GetCollider() const;
        Transform GetTransform() const;
        bool IsTrigger() const;
        std::function<void(const Collision&, float)> OnCollision() const;
        void SetPosition(const Vector2 &pos);
        void SetCollider(const std::shared_ptr<SquareCollider> collider);
        void SetCollider(const std::shared_ptr<CircleCollider> collider);
        void SetTransform(const Transform &transform);
        void SetTrigger(bool isTrigger);
    protected:
        std::shared_ptr<Collider> m_Collider;
        Transform m_Transform;
        bool m_IsTrigger;
        //must implement functionality
        std::function<void(const Collision&, float)> m_OnCollision;
    };
}