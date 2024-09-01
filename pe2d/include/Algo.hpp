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

    template <typename T>
    concept isContainer = requires (T c)
    {
        c.begin();
        c.end();
    };


    namespace algo
    {
        /* 
            Returns detailed information about a collision beetwen two circles:
            - The depth of penetration between the objects.
            - The normal vector along which the penetration occurred.
            - The contact point where the collision took place.
            - A boolean indicating whether a collision actually occured.
        */
        CollisionPoints FindCircleCircleCollision(
            std::shared_ptr<CircleCollider> circleA, Transform transformCircleA,
            std::shared_ptr<CircleCollider> circleB, Transform transformCircleB);

        /* 
            Returns detailed information about a collision beetwen circle and box:
            - The depth of penetration between the objects.
            - The normal vector along which the penetration occurred.
            - The contact point where the collision took place.
            - A boolean indicating whether a collision actually occured.
        */
        CollisionPoints FindCircleBoxCollision(
            std::shared_ptr<CircleCollider> circle, Transform transformCircle,
            std::shared_ptr<BoxCollider> box, Transform transformBox);

        /* 
            Returns detailed information about a collision beetwen box and circle:
            - The depth of penetration between the objects.
            - The normal vector along which the penetration occurred.
            - The contact point where the collision took place.
            - A boolean indicating whether a collision actually occured.
        */
        CollisionPoints FindBoxCircleCollision(
            std::shared_ptr<BoxCollider> box, Transform transformBox,
            std::shared_ptr<CircleCollider> circle, Transform transformCircle);

        /* 
            Returns detailed information about a collision beetwen two boxes:
            - The depth of penetration between the objects.
            - The normal vector along which the penetration occurred.
            - The contact points where the collision took place.
            - A boolean indicating whether a collision actually occured.
        */
        CollisionPoints FindBoxBoxCollision(
            std::shared_ptr<BoxCollider> boxA, Transform transformBoxA,
            std::shared_ptr<BoxCollider> boxB, Transform transformBoxB);
        
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
        requires isContainer<Container>
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
        
        /* 
            Indicates whether overlap occurred between the two projections of two edges and
            gives depth of overlap by modifying value of overlap parameter
        */
        constexpr bool Overlap(Vector2 A, Vector2 B, float &overlap)
        {
            if(A.y >= B.x && B.y >= A.x)
            {
                const float overlapStart = std::max(A.x, B.x);
                const float overlapEnd = std::min(A.y, B.y);
                overlap = overlapEnd - overlapStart;
                return true;
            }
            overlap = -1.0f;
            return false;
        }
        
        // Returns a collection of two normalized direction vectors representing the edges created by the given vertices.
        std::array<Vector2, 2> GetBoxAxes(const std::array<Vector2, 4> &vertices);
        
        // Returns normalized direction vector representing edge created by closest vertex and circle center
        template <typename Container>
        requires isContainer<Container>
        Vector2 GetCircleAxis(const Container &vertices, Vector2 circleCenter)
        {
            float dist = pe2dMath::INF;
            Vector2 smallestAxis = Vector2(0.0f, 0.0f);
            for(auto it = vertices.begin(); it != vertices.end(); it++)
            {
                const Vector2 edge = *it - circleCenter;
                const float d = pe2dMath::Length(edge);
                if(d < dist)
                {
                    dist = d;
                    smallestAxis = edge;
                }
            }
            return pe2dMath::Normalize(smallestAxis);
        }

        // Returns a projection of object, represented by given vertices, onto a specified axis
        template<typename Container>
        requires isContainer<Container>
        Vector2 Project(const Container &vertices, Vector2 axis)
        {
            float min = pe2dMath::Dot(axis, *vertices.begin());
            float max = min;

            for(auto it = std::next(vertices.begin()); it != vertices.end(); it++)
            {
                const float p = pe2dMath::Dot(axis, *it);
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