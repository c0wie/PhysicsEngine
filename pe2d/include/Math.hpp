#pragma once
#include <cmath>
#include "Vector2.hpp"

namespace pe2d
{
    constexpr float Length(Vector2 a)
    {
        return std::hypot(a.x, a.y);
    }

    constexpr float SquaredLength(Vector2 a)
    {
        return std::pow(Length(a), 2);
    }

    constexpr Vector2 Perp(Vector2 a)
    {
        return Vector2(-a.y, a.x);
    }
    
    constexpr Vector2 Normalize(Vector2 a)
    {
        const float length = Length(a);

        if(length == 0.0f)
        {
            return Vector2(0.0f, 0.0f);
        }

        return Vector2(a.x / length, a.y / length);
    }
    
    constexpr float Dot(Vector2 a, Vector2 b)
    {
        return a.x * b.x + a.y * b.y;
    }
    
    constexpr Vector2 Lerp(Vector2 a, Vector2 b, float time)
    {
        time = std::clamp(time, 0.0f, 1.0f);
        return Vector2{a * (1.0f - time) + b * time};
    }
};