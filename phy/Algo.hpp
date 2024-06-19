#pragma once

#include <memory>
#include "CollisionPoints.hpp"
#include "SphereCollider.hpp"
#include "PlaneCollider.hpp"

namespace phy
{
    namespace algo
    {
        CollisionPoints FindSphereSphereCollision(
            const SphereCollider *A, const Transform *transformA,
            const SphereCollider *B, const Transform *transformB)
        {
        
        }      
    
        CollisionPoints FindSpherePlaneCollision(
            const SphereCollider *A, const Transform *transformA,
            const PlaneCollider *B, const Transform *transformB)
        {
        
        }
    
        CollisionPoints FindPlaneSphereCollision(
            const PlaneCollider *A, const Transform *transformA,
            const SphereCollider *B, const Transform *transformB)
        {
        
        }        
    }
}