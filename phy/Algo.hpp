#pragma once

#include <memory>
#include "CircleCollider.hpp"
#include "SquareCollider.hpp"

namespace phy
{
    namespace algo
    {
        CollisionPoints FindCircleCircleCollision(
            const CircleCollider *A, const Transform *transformA,
            const CircleCollider *B, const Transform *transformB)
        {
            return CollisionPoints();
        }      
    
        CollisionPoints FindCircleSquareCollision(
            const CircleCollider *A, const Transform *transformA,
            const SquareCollider *B, const Transform *transformB)
        {
            return CollisionPoints();
        }
    
        CollisionPoints FindSquareCircleCollision(
            const SquareCollider *A, const Transform *transformA,
            const CircleCollider *B, const Transform *transformB)
        {
            return CollisionPoints();
        }        

        CollisionPoints FindSquareSquareCollision(
            const SquareCollider *A, const Transform *transformA,
            const SquareCollider *B, const Transform *transformB   )
        {
            return CollisionPoints();
        }
    }
}