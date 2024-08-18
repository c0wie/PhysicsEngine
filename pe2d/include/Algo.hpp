#pragma once

#include <vector>
#include <memory>
#include <limits>
#include "Math.hpp"
#include "CollisionPoints.hpp"
#include "Transform.hpp"

namespace pe2d
{
    class CircleCollider;
    class BoxCollider;

    namespace algo
    {
        constexpr float INF = std::numeric_limits<float>::infinity();
        constexpr float MIN = std::numeric_limits<float>::min();
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
        
        // angle in radians
        template <typename Container>
        void RotateVertices(Container &vertices, Vector2 center, float angle)
        {
            const float cosAngle = cosf(angle);
            const float sinAngle = sinf(angle);
            for(auto it = vertices.begin(); it != vertices.end(); it++)
            {
                const float relativeX = it->x - center.x;
                const float relativeY = it->y - center.y;

                float rotatedX = (relativeX * cosAngle) - (relativeY * sinAngle);
                float rotatedY = (relativeX * sinAngle) + (relativeY * cosAngle);
                *it = Vector2( rotatedX + center.x, rotatedY + center.y );
            }
        }
        
        // return box vertices in clockwise direction
        std::array<Vector2, 4> GetBoxVertices(Vector2 boxSize, Transform transform);
        
        //check if two projections overlap
        constexpr bool Overlap(Vector2 A, Vector2 B)
        {
            if(A.y >= B.x && B.y >= A.x)
            {
                return true;
            }
            return false;
        }
        
        // return overlap of two objects
        constexpr float GetOverlap(Vector2 A,  Vector2 B)
        {
            const float overlapStart = std::max(A.x, B.x);
            const float overlapEnd = std::min(A.y, B.y);

            return overlapEnd - overlapStart;
        }
        
        // return perpendicular and Normalize axes of an object's edges
        std::vector<Vector2> GetAxes(const std::vector<Vector2> &vertices);
        
        // return perpendicular and Normalize axes of a box edges
        std::array<Vector2, 2> GetBoxAxes(const std::array<Vector2, 4> &vertices);
        
        // project an shape on axis 
        template<typename Container>
        Vector2 Project(const Container &vertices, Vector2 axis)
        {
            float min = Dot(axis, *vertices.begin());
            float max = min;

            for(auto it = std::next(vertices.begin()); it != vertices.end(); it++)
            {
                const float p = Dot(axis, *it);
                if(p < min)
                {
                    min = p;
                }
                else if(p > max)
                {
                    max = p;
                }
            }
            return Vector2(min, max);
        }    
        
        // project circle on axis
        Vector2 ProjectCircle(Vector2 circleCenter, float radius, Vector2 axis);

        // return perpendicular and Normalize axis to circle center
        template <typename Container>
        Vector2 GetCircleAxis(const Container &vertices, Vector2 circleCenter)
        {
            float dist = INF;
            Vector2 smallestAxis = Vector2(0.0f, 0.0f);
            for(auto it = vertices.begin(); it != vertices.end(); it++)
            {
                const Vector2 edge = *it - circleCenter;
                const float d = Length(edge);
                if(d < dist)
                {
                    dist = d;
                    smallestAxis = edge;
                }
            }
            return Normalize(smallestAxis);
        }
    }
}