#pragma once

#include "Vector2.hpp"

namespace phy
{
    struct CollisionPoints
    {
    public:
        // PROBABLY DELETE this two points A andB cuz there are useles to me
        Vector2 A; // Furthest point of A into B
        Vector2 B; //Furthest point of B into A
        Vector2 Normal; //B-A Normalized
        float Depth; //Lenght of B-A
        bool HasCollision;
    public:
        constexpr CollisionPoints() :
            A(Vector2{}),  B(Vector2{}),  Normal(Vector2{}), Depth(0.0f), HasCollision(false) {}
        constexpr CollisionPoints(const Vector2 &a, const Vector2 &b, const Vector2 &normal, float depth, bool hasCollision) :
            A(a), B(b), Normal(normal), Depth(depth), HasCollision(hasCollision) {}
        constexpr CollisionPoints(const CollisionPoints &other) = default;
        constexpr CollisionPoints(CollisionPoints &&other) noexcept :
            A(other.A), B(other.B), Normal(other.Normal), Depth(other.Depth), HasCollision(other.HasCollision)
        {
            other.A = Vector2{};
            other.B = Vector2{};
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
            A = other.A;
            B = other.B;
            Normal = other.Normal;
            Depth = other.Depth;
            HasCollision = other.HasCollision;
            other.A = Vector2{0.0f, 0.0f};
            other.B = Vector2{0.0f, 0.0f};
            other.Normal = Vector2{0.0f, 0.0f};
            other.Depth = 0.0f;
            other.HasCollision = false;
            return *this;
        }
    };
}