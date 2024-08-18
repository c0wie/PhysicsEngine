#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Utilities.hpp"

namespace pe2d
{
    struct Vector2
    {
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
    public:
        constexpr bool operator==(Vector2 other) const
        {
            return x == other.x && y == other.y;
        }
        constexpr bool operator!=(Vector2 other) const
        {
            return x != other.x && y != other.y;
        }
        constexpr bool operator <=(Vector2 other) const
        {
            return (x <= other.x && y <= other.y);
        }
        constexpr Vector2 operator+(Vector2 other) const
        {
            return Vector2{ x + other.x, y + other.y};
        }
        constexpr Vector2 operator-(Vector2 other) const
        {
            return Vector2{x - other.x, y - other.y};
        }      
        constexpr Vector2& operator+=(Vector2 other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }
        constexpr Vector2& operator+=(float number)
        {
            x += number;
            y += number;
            return *this;
        }
        constexpr Vector2 operator*(Vector2 other) const
        {
            return Vector2{x * other.x, y * other.y};
        }
        constexpr Vector2 operator*(float multiplier) const
        {
            return Vector2{ x * multiplier, y * multiplier};
        }
        friend constexpr Vector2 operator*(float multiplier, const Vector2& vec)
        {
            return vec * multiplier;
        }
        constexpr Vector2& operator*=(float multiplier)
        {
            x *= multiplier;
            y *= multiplier;
            return *this;
        }
        constexpr Vector2 operator/(float divider) const
        {
             return Vector2{ x / divider, y / divider};
        }
        std::string GetString() const
        {
            std::string name = std::to_string(x) + " " + std::to_string(y);
            return name;
        }
    public:
        float x;
        float y;
    };
}