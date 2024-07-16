#pragma once

#include <vector>
#include <memory>
#include <limits>
#include "CollisionPoints.hpp"
#include "Transform.hpp"

namespace pe2d
{
    class CircleCollider;
    class BoxCollider;

    class Algo
    {
    public:
        static CollisionPoints FindCircleCircleCollision(
            const CircleCollider *circleA, const Transform &transformCircleA,
            const CircleCollider *circleB, const Transform &transformCircleB);
    
        static CollisionPoints FindCircleBoxCollision(
            const CircleCollider *circle, const Transform &transformCircle,
            const BoxCollider *box, const Transform &transformBox);
    
        static CollisionPoints FindBoxCircleCollision(
            const BoxCollider *box, const Transform &transformBox,
            const CircleCollider *circle, const Transform &transformCircle);

        static CollisionPoints FindBoxBoxCollision(
            const BoxCollider *boxA, const Transform &transformBoxA,
            const BoxCollider *boxB, const Transform &transformBoxB);
        static std::vector<Vector2> GetBoxVertices(const Vector2 &boxSize, const Transform &transform);
    private:
        static bool Overlap(const Vector2 &A, const Vector2 &B);
        static float GetOverlap(const Vector2 &A, const Vector2 &B);
        static std::vector<Vector2> GetAxes(const std::vector<Vector2>vertices);
        static Vector2 Project(const std::vector<Vector2> &vertices, const Vector2 &axis);
        static Vector2 ProjectCircle(const Vector2 &axis, const Vector2 &circleCenter, float radius);
        static void RotateVertices(std::vector<Vector2> &vertices, const Vector2 &center, float angle);
        static Vector2 GetCircleAxis(std::vector<Vector2> vertices, const Vector2 &circleCenter);
    private:
        static constexpr float INF = std::numeric_limits<float>::infinity();
    };
}