// might be colliding with SFML's version of Transform.hpp
#pragma once

#include "Vector2.hpp"
namespace phy
{
    class Transform
    {
    public:
        Vector2 Position;
        Vector2 Scale;
    public:
        Transform() :
            Position(Vector2(0.0f, 0.0f)), Scale(Vector2(0.0f, 0.0f)) {};
        Transform(const Vector2 &position, const Vector2 &scale) :
            Position(position), Scale(scale) {};
    };
}