#include "../headers/CollisionPoints.hpp"

namespace phy
{
    CollisionPoints::CollisionPoints() :
        A(Vector2{0.0f, 0.0f}),  B(Vector2{0.0f, 0.0f}),  Normal(Vector2{0.0f, 0.0f}), Depth(0.0f), HasCollision(false) {};
    
    CollisionPoints::CollisionPoints(const Vector2 &a, const Vector2 &b, const Vector2 &normal, float depth, bool hasCollision) :
        A(a), B(b), Normal(normal), Depth(depth), HasCollision(hasCollision) {};

    CollisionPoints::CollisionPoints(const CollisionPoints &other) :
        A(other.A), B(other.B), Normal(other.Normal), Depth(other.Depth), HasCollision(other.HasCollision) {};

    CollisionPoints::CollisionPoints(CollisionPoints &&other) noexcept :
        A(other.A), B(other.B), Normal(other.Normal), Depth(other.Depth), HasCollision(other.HasCollision) 
    {
        other.A = Vector2{0.0f, 0.0f};
        other.B = Vector2{0.0f, 0.0f};
        other.Normal = Vector2{0.0f, 0.0f};
        other.Depth = 0.0f;
        other.HasCollision = false;
    };

    CollisionPoints& CollisionPoints::operator=(const CollisionPoints &other)
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

    CollisionPoints& CollisionPoints::operator=(CollisionPoints &&other) noexcept
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
}