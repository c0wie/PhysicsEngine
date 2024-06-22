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
        CollisionPoints(Vector2 a, Vector2 b, Vector2 normal, float depth, bool hasCollision);
        CollisionPoints(const CollisionPoints &other);
        CollisionPoints(CollisionPoints &&other) noexcept;
        CollisionPoints& operator=(const CollisionPoints &other);
        CollisionPoints& operator=(CollisionPoints &&other) noexcept;
    };
}