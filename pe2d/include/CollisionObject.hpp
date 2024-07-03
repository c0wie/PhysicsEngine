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
        constexpr Transform GetTransform() const { return m_Transform;}
        constexpr bool IsTrigger() const { return m_IsTrigger; }
        std::function<void(const Collision&, float)> OnCollision() const;
        constexpr void SetPosition(const Vector2 &pos) { m_Transform.position = pos; }
        void SetCollider(const std::shared_ptr<SquareCollider> collider);
        void SetCollider(const std::shared_ptr<CircleCollider> collider);
        constexpr void SetTransform(const Transform &transform) { m_Transform = transform; }
        constexpr void SetTrigger(bool isTrigger) { m_IsTrigger = isTrigger; }

        constexpr void Move(const Vector2 &offset) { m_Transform.Move(offset); }
        constexpr void Rotate(float angle) { m_Transform.Rotate(angle); }
    protected:
        std::shared_ptr<Collider> m_Collider;
        Transform m_Transform;
        bool m_IsTrigger;
        //must implement functionality
        std::function<void(const Collision&, float)> m_OnCollision;
    };
}