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

    namespace algo
    {
        CollisionPoints FindCircleCircleCollision(
            const CircleCollider *circleA, const Transform &transformCircleA,
            const CircleCollider *circleB, const Transform &transformCircleB);
    
        CollisionPoints FindCircleBoxCollision(
            const CircleCollider *circle, const Transform &transformCircle,
            const BoxCollider *box, const Transform &transformBox);
    
        CollisionPoints FindBoxCircleCollision(
            const BoxCollider *box, const Transform &transformBox,
            const CircleCollider *circle, const Transform &transformCircle);

        CollisionPoints FindBoxBoxCollision(
            const BoxCollider *boxA, const Transform &transformBoxA,
            const BoxCollider *boxB, const Transform &transformBoxB);
        
        std::vector<Vector2> GetBoxVertices(const Vector2 &boxSize, const Transform &transform);
        bool Overlap(const Vector2 &A, const Vector2 &B);
        float GetOverlap(const Vector2 &A, const Vector2 &B);
        std::vector<Vector2> GetAxes(const std::vector<Vector2> &vertices);
        std::vector<Vector2> GetRectangleAxes(const std::vector<Vector2> &vertices);
        Vector2 Project(const std::vector<Vector2> &vertices, const Vector2 &axis);
        Vector2 ProjectCircle(const Vector2 &axis, const Vector2 &circleCenter, float radius);
        void RotateVertices(std::vector<Vector2> &vertices, const Vector2 &center, float angle);
        Vector2 GetCircleAxis(std::vector<Vector2> vertices, const Vector2 &circleCenter);
        constexpr float INF = std::numeric_limits<float>::infinity();
    }
}