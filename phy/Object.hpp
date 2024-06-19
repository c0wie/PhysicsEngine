#pragma once

#include <memory>
#include "Vector2.hpp"
#include "CollisionPoints.hpp"
#include "Transform.hpp"
#include "Collider.hpp"

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
        Object() :
            Mass(0.0f), Velocity(0.0f, 0.0f), Force(0.0f, 0.0f), Collider(nullptr), Transform(nullptr) {};

        Object(float mass, Vector2 velocity, Vector2 force, std::shared_ptr<phy::Collider> collider, std::shared_ptr<phy::Transform> transform) :
            Mass(mass), Velocity(velocity), Force(force), Collider(collider), Transform(transform) {};
    };
}