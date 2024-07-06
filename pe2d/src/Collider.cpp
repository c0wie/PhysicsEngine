#include "Collider.hpp"
#include "Algo.hpp"
namespace pe2d
{
#pragma region Circle Collider

    CollisionPoints CircleCollider::TestCollision(
        const Transform &transform,
        const Collider *collider,
        const Transform &colliderTransform) const 
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }
    
    CollisionPoints CircleCollider::TestCollision(
        const Transform &transform,
        const CircleCollider *circle,
        const Transform &circleTransform) const 
    {
        return Algo::FindCircleCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints CircleCollider::TestCollision(
        const Transform &transform,
        const SquareCollider *square,
        const Transform &squareTransform) const 
    {
        return Algo::FindCircleSquareCollision(this, transform, square, squareTransform);
    }

#pragma endregion

#pragma region Polygon Collider

    CollisionPoints SquareCollider::TestCollision(
        const Transform &transform,
        const Collider *collider,
        const Transform &colliderTransform) const 
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }

    CollisionPoints SquareCollider::TestCollision(
        const Transform &transform,
        const CircleCollider *circle,
        const Transform &circleTransform) const 
    {
        return Algo::FindSquareCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints SquareCollider::TestCollision(
        const Transform &transform,
        const SquareCollider *square,
        const Transform &squareTransform) const 
    {
        return Algo::FindSquareSquareCollision(this, transform, square, squareTransform);
    }

#pragma endregion
}