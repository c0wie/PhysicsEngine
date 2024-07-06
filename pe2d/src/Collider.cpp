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
        const PolygonCollider *polygon,
        const Transform &polygonTransform) const 
    {
        return Algo::FindCirclePolygonCollision(this, transform, polygon, polygonTransform);
    }

#pragma endregion

#pragma region Polygon Collider

    CollisionPoints PolygonCollider::TestCollision(
        const Transform &transform,
        const Collider *collider,
        const Transform &colliderTransform) const 
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }

    CollisionPoints PolygonCollider::TestCollision(
        const Transform &transform,
        const CircleCollider *circle,
        const Transform &circleTransform) const 
    {
        return Algo::FindPolygonCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints PolygonCollider::TestCollision(
        const Transform &transform,
        const PolygonCollider *polygon,
        const Transform &polygonTransform) const 
    {
        return Algo::FindPolygonPolygonCollision(this, transform, polygon, polygonTransform);
    }
#pragma endregion
}