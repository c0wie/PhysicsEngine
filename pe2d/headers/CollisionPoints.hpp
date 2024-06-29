#pragma once

#include "Vector2.hpp"

namespace phy
{
    struct CollisionPoints
    {
    public:
        Vector2 Normal; // normalized length of vector
        float Depth; // length of two objects overlap
        bool HasCollision;
    public:
        constexpr CollisionPoints() :
            Normal(Vector2{}), Depth(0.0f), HasCollision(false) {}
        constexpr CollisionPoints(const Vector2 &normal, float depth, bool hasCollision) :
            Normal(normal), Depth(depth), HasCollision(hasCollision) {}
        constexpr CollisionPoints(const CollisionPoints &other) = default;
        constexpr CollisionPoints(CollisionPoints &&other) noexcept :
            Normal(other.Normal), Depth(other.Depth), HasCollision(other.HasCollision)
        {
            other.Normal = Vector2{};
            other.Depth = 0.0f;
            other.HasCollision = false;
        }
        constexpr CollisionPoints& operator=(const CollisionPoints &other) = default;
        constexpr CollisionPoints& operator=(CollisionPoints &&other) noexcept
        {
            if(this == &other)
            {
                return *this;
            }
            Normal = other.Normal;
            Depth = other.Depth;
            HasCollision = other.HasCollision;
            other.Normal = Vector2{0.0f, 0.0f};
            other.Depth = 0.0f;
            other.HasCollision = false;
            return *this;
        }
    };
}