#pragma once
#include <iostream>

//#define DEBUG 
#define LogCall(x) 

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
        Vector2& operator+=(const Vector2 &other) ;
        Vector2& operator+=(float number);
        Vector2 operator*(float multiplier) const;
        Vector2 operator/(float divider) const;
    };
}