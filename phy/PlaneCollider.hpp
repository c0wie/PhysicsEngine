#pragma once

#include "Collider.hpp"
#include "Algo.hpp"

namespace phy
{
    class phy::PlaneCollider : public Collider
    {
    public:
        Vector2 Plane;
        float Distance;
    public:
        PlaneCollider() : 
            Plane(Vector2(0.0f, 0.0f)), Distance(0.0f) {};

        PlaneCollider(Vector2 plane, float distance) :
            Plane(plane), Distance(distance) {};

        PlaneCollider(const PlaneCollider &other) :
            Plane(other.Plane), Distance(other.Distance)
        {}

        PlaneCollider(PlaneCollider &&other)noexcept :
            Plane(other.Plane), Distance(other.Distance)
        {
            other.Plane = Vector2(0.0f, 0.0f);
            other.Distance = 0.0f;
        }

        PlaneCollider& operator=(const PlaneCollider &other)
        {
            if(this == &other)
            {
                return *this;
            }
            Plane = other.Plane;
            Distance = other.Distance;
            return *this;
        }

        PlaneCollider& operator=(PlaneCollider &&other)noexcept
        {
            if(this == &other)
            {
                return *this;
            }
            Plane = other.Plane;
            Distance = other.Distance;
            other.Plane = Vector2(0.0f, 0.0f);
            other.Distance = 0.0f;
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
            return algo::FindPlaneSphereCollision(this, transform, sphere, sphereTransform);
        }
        CollisionPoints TestCollision(
            const Transform *transform,
            const PlaneCollider *plane,
            const Transform *planeTransform) const override
        {
            //TODO: ASSERT THIS SHIT
            return {};
        }
    };
}