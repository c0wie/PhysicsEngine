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

    Vector2& Vector2::operator*=(float multiplier)
    {
        x *= multiplier;
        y *= multiplier;
        return *this;
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
        float length = sqrt(pow(x, 2) + pow(y, 2));
        return Vector2 {x / length, y / length};
    }

    float Vector2::dot(const Vector2 &other) const
    {
        return x * other.x + y * other.y;
    }

    float Vector2::magnitude() const
    {
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    Vector2 Vector2::lerp(const Vector2 &x, const Vector2 &y, float t)
    {
        t = std::clamp(t, 0.0f, 1.0f);
        return Vector2{x * (1.0f - t) + y * t};
    }

    Vector2 Vector2::abs() const
    {
        return Vector2{x < 0 ? -x : x, y < 0 ? -y : y,};
    }
}
