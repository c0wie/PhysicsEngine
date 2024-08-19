#pragma once
#include <cmath>
#include "Vector2.hpp"

namespace pe2d
{
    constexpr float Length(Vector2 a)
    {
        return std::sqrt(a.x * a.x + a.y * a.y);
    }

    constexpr float SquaredLength(Vector2 a)
    {
        return a.x * a.x + a.y * a.y;
    }

    constexpr float Distance(Vector2 a, Vector2 b)
    {
        const float distanceX = a.x - b.x;
        const float distanceY = a.y - b.y;
        return std::sqrt(distanceX * distanceX + distanceY * distanceY);
    }

    constexpr float SquaredDistance(Vector2 a, Vector2 b)
    {
        const float distanceX = a.x - b.x;
        const float distanceY = a.y - b.y;
        return distanceX * distanceX + distanceY * distanceY;
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

    constexpr bool NearlyEquel(float a, float b, float absError)
    {
        return std::abs(a - b) <= absError;
    }

    constexpr bool NearlyEquel(Vector2 a, Vector2 b, float absError)
    {
        return NearlyEquel(a.x , b.x, absError) && NearlyEquel(a.y, b.y, absError);
    }
};