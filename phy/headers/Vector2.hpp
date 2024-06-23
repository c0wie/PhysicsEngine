#pragma once
#include <iostream>

// #define DEBUG

#ifdef DEBUG
    #define LogCall(x) std::cout << x << std::endl;
#else
    #define LogCall(x) 
#endif


namespace phy
{
    class Vector2
    {
    public:
        float x;
        float y;
    public:
        Vector2();
        Vector2(float x, float y);
        Vector2(const Vector2 &other);
        Vector2(Vector2 &&other) noexcept;
        Vector2& operator=(const Vector2 &other);
        Vector2& operator=(Vector2 &&other)noexcept;

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