#pragma once
#include <iostream>
#include <algorithm>
#include "Assert.hpp"

namespace phy
{
    struct Vector2
    {
    public:
        float x;
        float y;
    public:
        constexpr Vector2() :
            x(0.0f), y(0.0f) {}
        constexpr Vector2(float x, float y) :
            x(x), y(y) {}
        constexpr Vector2(const Vector2 &other) = default;
        constexpr Vector2(Vector2 &&other) noexcept :
            x(other.x), y(other.y) 
        {
            other.x = 0.0f;
            other.y = 0.0f;
        }
        constexpr Vector2& operator=(const Vector2 &other) = default;
        constexpr Vector2& operator=(Vector2 &&other)noexcept
        {
            if(this == &other)
            {
                return *this;
            }
            x = other.x;
            y = other.y;
            other.x = 0.0f;
            other.y = 0.0f;
            return *this;
        }

        Vector2 operator+(const Vector2 &other) const;
        Vector2 operator-(const Vector2 &other) const;
        Vector2& operator+=(const Vector2 &other) ;
        Vector2& operator+=(float number);
        Vector2 operator*(float multiplier) const;
        Vector2 operator/(float divider) const;

        // returns perpendicular vector 
        Vector2 perp() const;
        // returns normalized vector
        Vector2 normalized() const;
        // returns dot product of two vectors;
        float dot(const Vector2 &other) const;

    };
}