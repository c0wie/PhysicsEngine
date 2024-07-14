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
        constexpr Vector2 operator+(const Vector2 &other) const
        {
            return Vector2{ x + other.x, y + other.y};
        }
        constexpr Vector2 operator-(const Vector2 &other) const
        {
            return Vector2{x - other.x, y - other.y};
        }      
        constexpr Vector2& operator+=(const Vector2 &other)
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
        constexpr Vector2 operator*(const Vector2 &other) const
        {
            return Vector2{x * other.x, y * other.y};
        }
        constexpr Vector2 operator*(float multiplier) const
        {
            return Vector2{ x * multiplier, y * multiplier};
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
        // returns perpendicular vector 
        constexpr Vector2 perp() const
        {
            return Vector2{-y, x};
        }
        // returns normalized vector
        constexpr Vector2 normalized() const
        {
            float length = magnitude();
            return Vector2 {x / length, y / length};
        }
        // returns dot product of two vectors;
        constexpr float dot(const Vector2 &other) const
        {
            return x * other.x + y * other.y;
        }
        //returns length of vector
        constexpr float magnitude() const
        {
            //std::hypot == sqrt(pow(x, 2) + pow(y, 2));
            return std::hypot(x, y);
        }
        //return positvie value of vector
        constexpr static Vector2 abs(const Vector2 vec)
        {
            return Vector2{vec.x < 0 ? -vec.x : vec.x, vec.y < 0 ? -vec.y : vec.y};
        }
        constexpr static Vector2 lerp(const Vector2 &x, const Vector2 &y, float t)
        {
            t = std::clamp(t, 0.0f, 1.0f);
            return Vector2{x * (1.0f - t) + y * t};
        }
        std::string GetString()
        {
            std::string name = "Vector2: " + std::to_string(static_cast<int>(x)) + " " + std::to_string(static_cast<int>(y)) + '\n';
            return name;
        }
    public:
        float x;
        float y;
    };
}