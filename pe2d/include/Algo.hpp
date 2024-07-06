#pragma once

#include <vector>
#include <memory>
#include <limits>
#include "CollisionPoints.hpp"
#include "Transform.hpp"

namespace pe2d
{
    class CircleCollider;
    class PolygonCollider;

    class Algo
    {
    public:
        static CollisionPoints FindCircleCircleCollision(
            const CircleCollider *A, const Transform &transformA,
            const CircleCollider *B, const Transform &transformB);
    
        static CollisionPoints FindCirclePolygonCollision(
            const CircleCollider *A, const Transform &transformA,
            const PolygonCollider *B, const Transform &transformB);
    
        static CollisionPoints FindPolygonCircleCollision(
            const PolygonCollider *A, const Transform &transformA,
            const CircleCollider *B, const Transform &transformB);

        static CollisionPoints FindPolygonPolygonCollision(
            const PolygonCollider *A, const Transform &transformA,
            const PolygonCollider *B, const Transform &transformB);
    private:
        static bool Overlap(const Vector2 &A, const Vector2 &B);
        static float GetOverlap(const Vector2 &A, const Vector2 &B);
        static void GetAxes(Vector2 *axes, Vector2 *vertecies, int size);
        static Vector2 Project(const Vector2 &axis, const Vector2 *vertecies, int size);
        static Vector2 ProjectCircle(const Vector2 &axis, const Vector2 &circleCenter, float radius);
        static void RotateVertecies(Vector2 *vertecies, unsigned int size, const Vector2 &center, float angle);
        static void GetPolygonVertecies(Vector2 *vertecies, unsigned int size, const Vector2 &center, float sideLength, const Vector2 &scale, float angle);
        static Vector2 GetCircleAxis(const Vector2 *vertecies, int size, const Vector2 &circleCenter);
    public:
        static constexpr float INF = std::numeric_limits<float>::infinity();
    };
}