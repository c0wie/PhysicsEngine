#pragma once

#include "Vector2.hpp"

namespace pe2d
{
    // struct which holds info about collision
    struct CollisionPoints
    {
        Vector2 Normal; // normalized length of vector B-A
        float Depth; // length of overlap beetwen two objects
        bool HasCollision;
    };
}