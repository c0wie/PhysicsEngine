#pragma once

#include "Vector2.hpp"

namespace phy
{
    class CollisionPoints
    {
    public:
        Vector2 A; // Furthest point of A into B
        Vector2 B; //Furthest point of B into A
        Vector2 Normal; //B-A Normalized
        float Depth; //Lenght of B-A
        bool HasCollision;
    public:
        CollisionPoints();
        CollisionPoints(const Vector2 &a, const Vector2 &b, const Vector2 &normal, float depth, bool hasCollision);
        CollisionPoints(const CollisionPoints &other);
        CollisionPoints(CollisionPoints &&other) noexcept;
        CollisionPoints& operator=(const CollisionPoints &other);
        CollisionPoints& operator=(CollisionPoints &&other) noexcept;
    };
}