#pragma once

#include <memory>
#include "CircleCollider.hpp"
#include "SquareCollider.hpp"

namespace phy
{
    class Object
    {
    public:
        float Mass;
        Vector2 Velocity;
        Vector2 Force;
        std::shared_ptr<Collider> Collider;
        std::shared_ptr<Transform> Transform;
    public:
        Object();
        Object(float mass, const Vector2 &velocity, const Vector2 &force, 
            const std::shared_ptr<phy::SquareCollider> &collider, const std::shared_ptr<phy::Transform> &transform);
        Object(float mass, const Vector2 &velocity, const Vector2 &force, 
            const std::shared_ptr<phy::CircleCollider> &collider, const std::shared_ptr<phy::Transform> &transform);
        Object (const Object &other);
        Object (Object &&other) noexcept;
        Object& operator=(const Object &other);
        Object& operator=(Object &&other) noexcept;
    };
}