#pragma once

#include <vector>
#include <memory>
#include <limits>
#include "CollisionPoints.hpp"
#include "Transform.hpp"

namespace phy
{
    constexpr float INF = std::numeric_limits<float>::infinity();
    class CircleCollider;
    class SquareCollider;

    class Algo
    {
    private:
        static bool Overlap(const Vector2 &A, const Vector2 &B);
        static float GetOverlap(const Vector2 &A, const Vector2 &B);
        static std::vector<Vector2> GetAxes(const std::vector<Vector2> &vertecies);
        static Vector2 Project(const Vector2 &axis, const std::vector<Vector2> &vertecies);
        static std::vector<Vector2> GetSquareVertecies(const Vector2 &center, float sideLength);
        static Vector2 FindFurthestPoint(const std::vector<Vector2> &vertecies, const Vector2 &direction);
    public:
        static CollisionPoints FindCircleCircleCollision(
            const CircleCollider *A, const Transform *transformA,
            const CircleCollider *B, const Transform *transformB);
    
        static CollisionPoints FindCircleSquareCollision(
            const CircleCollider *A, const Transform *transformA,
            const SquareCollider *B, const Transform *transformB);
    
        static CollisionPoints FindSquareCircleCollision(
            const SquareCollider *A, const Transform *transformA,
            const CircleCollider *B, const Transform *transformB);

        static CollisionPoints FindSquareSquareCollision(
            const SquareCollider *A, const Transform *transformA,
            const SquareCollider *B, const Transform *transformB);
    };
}