#pragma once

#include <memory>
#include "CollisionPoints.hpp"

namespace phy
{
    class PlaneCollider;
    class SphereCollider;
    namespace algo
    {
        CollisionPoints FindSphereSphereCollision(
            const SphereCollider *A, const Transform *transformA,
            const SphereCollider *B, const Transform *transformB)
        {
            return CollisionPoints();
        }      
    
        CollisionPoints FindSpherePlaneCollision(
            const SphereCollider *A, const Transform *transformA,
            const PlaneCollider *B, const Transform *transformB)
        {
            return CollisionPoints();
        }
    
        CollisionPoints FindPlaneSphereCollision(
            const PlaneCollider *A, const Transform *transformA,
            const SphereCollider *B, const Transform *transformB)
        {
            return CollisionPoints();
        }        
    }
}