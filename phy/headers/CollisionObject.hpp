#pragma once

#include <memory>
#include <functional>

#include "Collision.hpp"
#include "CircleCollider.hpp"
#include "SquareCollider.hpp"

namespace phy
{
    struct CollisionObject
    {
    protected:
        std::shared_ptr<Collider> m_Collider;
        std::shared_ptr<Transform> m_Transform;
        bool m_IsTrigger;
        std::function<void(Collision, float)> m_OnCollision;
    public:
        CollisionObject();
        // I might have to create seperate constructors for circle and square
        CollisionObject(const std::shared_ptr<Collider> collider, const std::shared_ptr<Transform> transform, bool trigger);
        CollisionObject(const CollisionObject &other);
        CollisionObject(CollisionObject &&other) noexcept;
        CollisionObject& operator=(const CollisionObject &other);
        CollisionObject& operator=(CollisionObject &&other);

        const std::shared_ptr<Collider> GetCollider() const;
        const std::shared_ptr<Transform> GetTransform() const;
        bool GetTrigger() const;
        // consider checking if collider isn't pure virtual class
        void SetCollider(const std::shared_ptr<Collider> collider);
        void SetTransform(const std::shared_ptr<Transform> transform);
        void SetTrigger(bool isTrigger);
    };
}