#pragma once

#include "Vector2.hpp"

namespace pe2d
{
    // struct which holds info about collision
    struct CollisionPoints
    {
    public:
        CollisionPoints() :
            Normal(Vector2(0.0f, 0.0f)), Depth(0.0f), ContactPoint1(Vector2(0.0f, 0.0f)), ContactPoint2(Vector2(0.0f, 0.0f)), HasCollision(false){}
        CollisionPoints(Vector2 normal, float depth, Vector2 contactPoint1, Vector2 contactPoint2, int contactCount, bool hasCollision) :
            Normal(normal), Depth(depth), ContactPoint1(contactPoint1), ContactPoint2(contactPoint2), HasCollision(hasCollision) {}
        CollisionPoints(const CollisionPoints &other) = default;
        CollisionPoints(CollisionPoints &&other) noexcept = default;
        CollisionPoints& operator=(const CollisionPoints &other) = default;
        CollisionPoints& operator=(CollisionPoints &&other)noexcept = default;
    public:
        Vector2 Normal; // normalized length of vector B-A
        float Depth; // length of overlap beetwen two objects
        Vector2 ContactPoint1;
        Vector2 ContactPoint2;
        int ContactCount;
        bool HasCollision;
    };
}