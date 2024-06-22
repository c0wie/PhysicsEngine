#pragma once

#include "Vector2.hpp"

namespace phy
{
    class CollisionPoints
    {
    public:
        Vector2 A; // Furthest point of A into B
        Vector2 B; //Furthest point of B into A
        Vector2 Normal; //B-A Normalized
        float Depth; //Lenght of B-A
        bool HasCollision;
    public:
        CollisionPoints() :
            A(Vector2{0.0f, 0.0f}),  B(Vector2{0.0f, 0.0f}),  Normal(Vector2{0.0f, 0.0f}), Depth(0.0f), HasCollision(false) {};
        
        CollisionPoints(Vector2 a, Vector2 b, Vector2 normal, float depth, bool hasCollision) :
            A(a), B(b), Normal(normal), Depth(depth), HasCollision(hasCollision) {};

        CollisionPoints(const CollisionPoints &other) :
            A(other.A), B(other.B), Normal(other.Normal), Depth(other.Depth), HasCollision(other.HasCollision) {};

        CollisionPoints(CollisionPoints &&other) noexcept :
            A(other.A), B(other.B), Normal(other.Normal), Depth(other.Depth), HasCollision(other.HasCollision) 
        {
            other.A = Vector2{0.0f, 0.0f};
            other.B = Vector2{0.0f, 0.0f};
            other.Normal = Vector2{0.0f, 0.0f};
            other.Depth = 0.0f;
            other.HasCollision = false;
        };

        CollisionPoints& operator=(const CollisionPoints &other)
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
            return *this;
        }

        CollisionPoints& operator=(CollisionPoints &&other) noexcept
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