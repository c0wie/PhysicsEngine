#pragma once

namespace phy
{
    class Vector2
    {
    public:
        float x;
        float y;
    public:
        Vector2() :
            x(0.0f), y(0.0f) {};

        Vector2(float x, float y) :
            x(x), y(y) {};

        Vector2(const Vector2 &other) :
            x(other.x), y(other.y)
            {};

        Vector2(Vector2 &&other) noexcept :
            x(other.x), y(other.y) {};

        Vector2& operator=(const Vector2 &other)
        {
            if(this == &other)
            {
                return *this;
            }
            x = other.x;
            y = other.y;
            return *this;
        }

        Vector2& operator=(Vector2 &&other)noexcept
        {
            if(this == &other)
            {
                return *this;
            }
            x = other.x;
            y = other.y;
            other.x = 0;
            other.y = 0;
            return *this;
        }

        Vector2 operator+(const Vector2 &other) const
        {
           return Vector2{ x + other.x, y + other.y};
        }


        Vector2& operator+=(const Vector2 &other) 
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2& operator+=(float number)
        {
            x += number;
            y += number;
            return *this;
        }

        Vector2 operator*(float multiplier) const
        {
            return Vector2{ x * multiplier, y * multiplier};
        }

        Vector2 operator/(float divider) const
        {
            return Vector2{ x / divider, y / divider};
        }
    };
}