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
    };
}