#pragma once

#include <memory>
#include <functional>

#include "Collider.hpp"

class Collision;
namespace phy
{
    class CollisionObject
    {
    protected:
        std::shared_ptr<Collider> m_Collider;
        std::shared_ptr<Transform> m_Transform;
        bool m_IsTrigger;
        std::function<void(const Collision&, float)> m_OnCollision = nullptr;
    public:
        CollisionObject();
        CollisionObject(const std::shared_ptr<CircleCollider> collider, const std::shared_ptr<Transform> transform, bool trigger);
        CollisionObject(const std::shared_ptr<SquareCollider> collider, const std::shared_ptr<Transform> transform, bool trigger);
        CollisionObject(const CollisionObject &other);
        CollisionObject(CollisionObject &&other) noexcept;
        CollisionObject& operator=(const CollisionObject &other);
        CollisionObject& operator=(CollisionObject &&other) noexcept;
        virtual ~CollisionObject() = default;

        const std::shared_ptr<Collider> GetCollider() const;
        const std::shared_ptr<Transform> GetTransform() const;
        bool IsTrigger() const;
        std::function<void(const Collision&, float)> OnCollision() const;
        // consider checking if collider isn't pure virtual class
        void SetPosition(const Vector2 &pos);
        void SetCollider(const std::shared_ptr<Collider> collider);
        void SetTransform(const std::shared_ptr<Transform> transform);
        void SetTrigger(bool isTrigger);
    };
}