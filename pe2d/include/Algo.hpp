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
        // returns info about circle - circle collision 
        // returns empty CollisionPoints if collision doesn't occur
        CollisionPoints FindCircleCircleCollision(
            const CircleCollider *circleA, Transform transformCircleA,
            const CircleCollider *circleB, Transform transformCircleB);

        // returns info about circle - box collision
        // returns empty CollisionPoints if collision doesn't occur
        CollisionPoints FindCircleBoxCollision(
            const CircleCollider *circle, Transform transformCircle,
            const BoxCollider *box, Transform transformBox);

        // returns info about box - circle collision 
        // returns empty CollisionPoints if collision doesn't occur
        CollisionPoints FindBoxCircleCollision(
            const BoxCollider *box, Transform transformBox,
            const CircleCollider *circle, Transform transformCircle);

        // returns info about box - box collision 
        // returns empty CollisionPoints if collision doesn't occur
        CollisionPoints FindBoxBoxCollision(
            const BoxCollider *boxA, Transform transformBoxA,
            const BoxCollider *boxB, Transform transformBoxB);
        
        std::vector<Vector2> GetBoxVertices(Vector2 boxSize, Transform transform);
        
        //checks if two projections overlap
        constexpr bool Overlap(Vector2 A, Vector2 B)
        {
            if(A.y >= B.x && B.y >= A.x)
            {
                return true;
            }
            return false;
        }
        
        // returns overlap of two objects
        constexpr float GetOverlap(Vector2 A,  Vector2 B)
        {
            const float overlapStart = std::max(A.x, B.x);
            const float overlapEnd = std::min(A.y, B.y);

            return overlapEnd - overlapStart;
        }
        
        // returns perpendicular and normalized axes of an object's edges
        std::vector<Vector2> GetAxes(const std::vector<Vector2> &vertices);
        
        // returns perpendicular and normalized axes of an rectangle
        std::vector<Vector2> GetRectangleAxes(const std::vector<Vector2> &vertices);
        
        // project an shape on axis 
        Vector2 Project(const std::vector<Vector2> &vertices, Vector2 axis);
        
        // project circle on axis
        Vector2 ProjectCircle(Vector2 axis, Vector2 circleCenter, float radius);
        
        void RotateVertices(std::vector<Vector2> &vertices, Vector2 center, float angle);
        
        // returns perpendicular and normalized axis to circle center
        Vector2 GetCircleAxis(const std::vector<Vector2> &vertices, Vector2 circleCenter);
        constexpr float INF = std::numeric_limits<float>::infinity();
    }
}