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
            const CircleCollider *circleA, Transform transformCircleA,
            const CircleCollider *circleB, Transform transformCircleB);
    
        CollisionPoints FindCircleBoxCollision(
            const CircleCollider *circle, Transform transformCircle,
            const BoxCollider *box, Transform transformBox);
    
        CollisionPoints FindBoxCircleCollision(
            const BoxCollider *box, Transform transformBox,
            const CircleCollider *circle, Transform transformCircle);

        CollisionPoints FindBoxBoxCollision(
            const BoxCollider *boxA, Transform transformBoxA,
            const BoxCollider *boxB, Transform transformBoxB);
        
        std::vector<Vector2> GetBoxVertices(Vector2 boxSize, Transform transform);
        bool Overlap(Vector2 A, Vector2 B);
        float GetOverlap(Vector2 A,  Vector2 B);
        std::vector<Vector2> GetAxes(const std::vector<Vector2> &vertices);
        std::vector<Vector2> GetRectangleAxes(const std::vector<Vector2> &vertices);
        Vector2 Project(const std::vector<Vector2> &vertices, Vector2 axis);
        Vector2 ProjectCircle(Vector2 axis, Vector2 circleCenter, float radius);
        void RotateVertices(std::vector<Vector2> &vertices, Vector2 center, float angle);
        Vector2 GetCircleAxis(std::vector<Vector2> &vertices, Vector2 circleCenter);
        constexpr float INF = std::numeric_limits<float>::infinity();
    }
}