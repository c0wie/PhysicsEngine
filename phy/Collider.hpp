#pragma once

#include <memory>
#include "Vector2.hpp"
#include "CollisionPoints.hpp"
#include "Transform.hpp"


namespace phy
{
    class PlaneCollider;
    class SphereCollider;
    
    class Collider 
    {
    public:
    Collider() = default;
        virtual CollisionPoints TestCollision(
            const Transform *transform,
            const Collider *collider,
            const Transform *colliderTransform) const = 0;
        
        virtual CollisionPoints TestCollision(
            const Transform *transform,
            const SphereCollider *sphere,
            const Transform *sphereTransform) const = 0;
        
        virtual CollisionPoints TestCollision(
            const Transform *transform,
            const PlaneCollider *plane,
            const Transform *planeTransform) const = 0;
    };
}