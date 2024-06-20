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

        Transform(const Transform &other) :
            Position(other.Position), Scale(other.Scale) {};

        Transform(Transform &&other) :
            Position(other.Position), Scale(other.Scale)
        {
            other.Position = Vector2(0.0f, 0.0f);
            other.Scale = Vector2(0.0f, 0.0f);
        };

        Transform& operator=(const Transform &other)
        {
            if(this == &other)
            {
                return *this;
            }
            Position = other.Position;
            Scale = other.Scale;
            return *this;
        }

        Transform& operator=(Transform &&other)
        {
            if(this == &other)
            {
                return *this;
            }
            Position = other.Position;
            Scale = other.Scale;
            other.Position = Vector2(0.0f, 0.0f);
            other.Scale = Vector2(0.0f, 0.0f);
            return *this;
        }
    };
}