#pragma once

#include <memory>
#include "CollisionPoints.hpp"
#include "Transform.hpp"

namespace phy
{
    class CircleCollider;
    class SquareCollider;
    namespace algo
    {
        CollisionPoints FindCircleCircleCollision(
            const CircleCollider *A, const Transform *transformA,
            const CircleCollider *B, const Transform *transformB);
    
        CollisionPoints FindCircleSquareCollision(
            const CircleCollider *A, const Transform *transformA,
            const SquareCollider *B, const Transform *transformB);
    
        CollisionPoints FindSquareCircleCollision(
            const SquareCollider *A, const Transform *transformA,
            const CircleCollider *B, const Transform *transformB);

        CollisionPoints FindSquareSquareCollision(
            const SquareCollider *A, const Transform *transformA,
            const SquareCollider *B, const Transform *transformB);
    }
}