#pragma once

#include "Vector2.hpp"

namespace pe2d
{
    /*
        Struct holding information about collision points:
        - `Depth`: The depth of collison.
        - `Normal`: The normal vector along which collision occured.
        - `ContactPoint1`: The first contact point of the collison.
        - `ContactPoint2`: The second contact point of the collision, if applicable.
        - `ContactCount`: The count of valid contact pointss.
        - `HasCollision`: A boolean indicating whether a collision actually occured.
        The second contact point (`ContactPoint2`) may be invalid or unused in some cases,
        so `contactCount` should be used to determine how many contact points are considered valid.
    */
    struct CollisionPoints
    {
    public:
        CollisionPoints() :
            Normal(Vector2(0.0f, 0.0f)), Depth(0.0f), ContactPoint1(Vector2(0.0f, 0.0f)),
            ContactPoint2(Vector2(0.0f, 0.0f)), ContactCount(0), HasCollision(false)
        {}

        CollisionPoints(Vector2 normal, float depth, Vector2 contactPoint1, Vector2 contactPoint2, bool hasCollision) :
            Normal(normal), Depth(depth), ContactPoint1(contactPoint1),
            ContactPoint2(contactPoint2), ContactCount(2), HasCollision(hasCollision)
        {}

        CollisionPoints(Vector2 normal, float depth, Vector2 contactPoint1, bool hasCollision) :
            Normal(normal), Depth(depth), ContactPoint1(contactPoint1),
            ContactPoint2(Vector2(0.0f, 0.0f)), ContactCount(1), HasCollision(hasCollision)
        {}
        CollisionPoints(const CollisionPoints &other) = default;
        CollisionPoints(CollisionPoints &&other) noexcept = default;
        CollisionPoints& operator=(const CollisionPoints &other) = default;
        CollisionPoints& operator=(CollisionPoints &&other)noexcept = default;
    public:
        float Depth; // length of overlap beetwen two objects
        Vector2 Normal; // normalized length of vector B-A
        Vector2 ContactPoint1;
        Vector2 ContactPoint2;
        int ContactCount;
        bool HasCollision;
    };
}