#include "../headers/Vector2.hpp"
#include <cmath>

namespace phy
{
    Vector2 Vector2::operator+(const Vector2 &other) const
    {
       return Vector2{ x + other.x, y + other.y};
    }

    Vector2 Vector2::operator-(const Vector2 &other) const
    {
        return Vector2{x - other.x, y - other.y};
    }
    
    Vector2& Vector2::operator+=(const Vector2 &other) 
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& Vector2::operator+=(float number)
    {
        x += number;
        y += number;
        return *this;
    }

    Vector2 Vector2::operator*(float multiplier) const
    {
        return Vector2{ x * multiplier, y * multiplier};
    }

    Vector2 Vector2::operator/(float divider) const
    {
        return Vector2{ x / divider, y / divider};
    }

    Vector2 Vector2::perp() const
    {
        return Vector2{-y, x};
    }
    
    Vector2 Vector2::normalized() const
    {
        float length = sqrt(pow(x, 2.0f) + pow(y, 2.0f));
        return Vector2 {x / length, y / length};
    }

    float Vector2::dot(const Vector2 &other) const
    {
        return x * other.x + y * other.y;
    }
}
