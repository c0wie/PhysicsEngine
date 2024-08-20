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
        /* 
            Returns detailed information about a collision beetwen two circles:
            - The depth of penetration between the objects.
            - The normal vector along which the penetration occurred.
            - The contact points where the collision took place.
            - A boolean indicating whether a collision actually occured.
        */
        CollisionPoints FindCircleCircleCollision(
            const CircleCollider *circleA, Transform transformCircleA,
            const CircleCollider *circleB, Transform transformCircleB);

        /* 
            Returns detailed information about a collision beetwen circle and box:
            - The depth of penetration between the objects.
            - The normal vector along which the penetration occurred.
            - The contact points where the collision took place.
            - A boolean indicating whether a collision actually occured.
        */
        CollisionPoints FindCircleBoxCollision(
            const CircleCollider *circle, Transform transformCircle,
            const BoxCollider *box, Transform transformBox);

        /* 
            Returns detailed information about a collision beetwen box and circle:
            - The depth of penetration between the objects.
            - The normal vector along which the penetration occurred.
            - The contact points where the collision took place.
            - A boolean indicating whether a collision actually occured.
        */
        CollisionPoints FindBoxCircleCollision(
            const BoxCollider *box, Transform transformBox,
            const CircleCollider *circle, Transform transformCircle);

        /* 
            Returns detailed information about a collision beetwen two boxes:
            - The depth of penetration between the objects.
            - The normal vector along which the penetration occurred.
            - The contact points where the collision took place.
            - A boolean indicating whether a collision actually occured.
        */
        CollisionPoints FindBoxBoxCollision(
            const BoxCollider *boxA, Transform transformBoxA,
            const BoxCollider *boxB, Transform transformBoxB);
        
        // Returns information about contact point in a collision beetwen box and circle
        Vector2 FindCircleBoxContactPoint(const std::array<Vector2, 4> &boxVertices, Vector2 circleCenter);
        
        // Returns information about contact points in a collision beetwen two boxes
        std::pair<Vector2, Vector2> FindBoxBoxContactPoint(const std::array<Vector2, 4> &boxVerticesA, const std::array<Vector2, 4> &boxVerticesB);

        /* 
            Modifies values of two output parameters: distanceSqured and contactPoint.
            - contactPoint - The point on the edge between vertexA and vertexB that is nearest to the given point.
            - distanceSquared - The squared distance between the given point and the nearest contactPoint.
        */
        void PointSegmentDistance(Vector2 point, Vector2 vertexA, Vector2 vertexB, float &distanceSquared, Vector2 &contactPoint);

        template <typename Container>
        void RotateVertices(Container &vertices, Vector2 center, float angleRadians)
        {
            const float cosAngle = cosf(angleRadians);
            const float sinAngle = sinf(angleRadians);
            for(auto it = vertices.begin(); it != vertices.end(); it++)
            {
                const float relativeX = it->x - center.x;
                const float relativeY = it->y - center.y;

                float rotatedX = (relativeX * cosAngle) - (relativeY * sinAngle);
                float rotatedY = (relativeX * sinAngle) + (relativeY * cosAngle);
                *it = Vector2( rotatedX + center.x, rotatedY + center.y );
            }
        }
        
        /*
            Returns a collection of four vertices representing the corners of a box 
            in clockwise order, based on the given box size and transform.
        */
        std::array<Vector2, 4> GetBoxVertices(Vector2 boxSize, Transform transform);
        
        // Indicates whether overlap occurred between the two projections of two edges.
        constexpr bool Overlap(Vector2 A, Vector2 B)
        {
            if(A.y >= B.x && B.y >= A.x)
            {
                return true;
            }
            return false;
        }
        
        // Returns length of the overlap between the two projections of two edges.
        constexpr float GetOverlap(Vector2 A,  Vector2 B)
        {
            const float overlapStart = std::max(A.x, B.x);
            const float overlapEnd = std::min(A.y, B.y);

            return overlapEnd - overlapStart;
        }
        
        // Returns a collection of normalized direction vectors representing the edges created by the given vertices.
        std::vector<Vector2> GetAxes(const std::vector<Vector2> &vertices);
        
        // Returns a collection of two normalized direction vectors representing the edges created by the given vertices.
        std::array<Vector2, 2> GetBoxAxes(const std::array<Vector2, 4> &vertices);
        
        // Returns normalized direction vector representing edge created by closest vertex and circle center
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

        // Returns a projection of object, represented by given vertices, onto a specified axis
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
        
        // Returns a projection of circle, represented by given circle center and radius, onto a specified axis
        Vector2 ProjectCircle(Vector2 circleCenter, float radius, Vector2 axis);
    }
}