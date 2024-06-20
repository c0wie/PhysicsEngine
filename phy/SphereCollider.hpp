#pragma once

#include "Collider.hpp"
#include "Algo.hpp"

// I should maybe reconsider using raw pointers for that cuz it is all const so what bad could happend...
// said everybody using raw pointers
namespace phy
{
    class SphereCollider : public Collider, std::enable_shared_from_this<SphereCollider>
    {
    public:
        Vector2 Center;
        float Radius;
    public:
        SphereCollider() : 
        Center(Vector2(0.0f, 0.0f)), Radius(0.0f) {};
    
        SphereCollider(const Vector2 &Center, float radius) :
            Center(Center), Radius(radius) {};

        SphereCollider(const SphereCollider &other) :
            Center(other.Center), Radius(other.Radius)
        {}

        SphereCollider(SphereCollider &&other)noexcept :
            Center(other.Center), Radius(other.Radius)
        {
            other.Center = Vector2(0.0f, 0.0f);
            other.Radius = 0.0f;
        }

        SphereCollider& operator=(const SphereCollider &other)
        {
            if(this == &other)
            {
                return *this;
            }
            Center = other.Center;
            Radius = other.Radius;
            return *this;
        }

        SphereCollider& operator=(SphereCollider &&other)noexcept
        {
            if(this == &other)
            {
                return *this;
            }
            Center = other.Center;
            Radius = other.Radius;
            other.Center = Vector2(0.0f, 0.0f);
            other.Radius = 0.0f;
            return *this;
        }

        CollisionPoints TestCollision(
            const Transform *transform,
            const Collider *collider,
            const Transform *colliderTransform) const override
        {
            return collider->TestCollision(colliderTransform, this, transform);
        }
        
        CollisionPoints TestCollision(
            const Transform *transform,
            const SphereCollider *sphere,
            const Transform *sphereTransform) const override
        {
            return algo::FindSphereSphereCollision(this, transform, sphere, sphereTransform);
        }

        CollisionPoints TestCollision(
            const Transform *transform,
            const PlaneCollider *plane,
            const Transform *planeTransform) const override
        {
            return algo::FindSpherePlaneCollision(this, transform, plane, planeTransform);
        }
    };
}