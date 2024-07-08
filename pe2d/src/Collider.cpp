#include "Collider.hpp"
#include "Algo.hpp"
namespace pe2d
{
#pragma region Circle Collider

    CollisionPoints CircleCollider::TestCollision(const Transform &transform, const Collider *collider, const Transform &colliderTransform) const 
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }
    
    CollisionPoints CircleCollider::TestCollision(const Transform &transform, const CircleCollider *circle, const Transform &circleTransform) const 
    {
        return Algo::FindCircleCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints CircleCollider::TestCollision(const Transform &transform, const ConvexShapeCollider *convexShape, const Transform &convexShapeTransform) const 
    {
        return Algo::FindCircleConvexShapeCollision(this, transform, convexShape, convexShapeTransform);
    }

    CollisionPoints CircleCollider::TestCollision(const Transform &transform, const BoxCollider *box, const Transform &boxTransform) const
    {
        return Algo::FindCircleBoxCollision(this, transform, box, boxTransform);
    }

#pragma endregion

#pragma region Box Collider

    CollisionPoints BoxCollider::TestCollision(const Transform &transform, const Collider *collider, const Transform &colliderTransform) const
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }

    CollisionPoints BoxCollider::TestCollision(const Transform &transform, const CircleCollider *circle, const Transform &circleTransform) const
    {
        return Algo::FindBoxCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints BoxCollider::TestCollision(const Transform &transform, const ConvexShapeCollider *convexShape, const Transform &convexShapeTransform) const
    {
        return Algo::FindBoxConvexShapeCollision(this, transform, convexShape, convexShapeTransform);
    }

    CollisionPoints BoxCollider::TestCollision(const Transform &transform, const BoxCollider *box, const Transform &boxTransform) const
    {
        return Algo::FindBoxBoxCollision(this, transform, box, boxTransform);
    }

#pragma endregion

#pragma region Polygon Collider

    CollisionPoints ConvexShapeCollider::TestCollision(const Transform &transform, const Collider *collider, const Transform &colliderTransform) const 
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }

    CollisionPoints ConvexShapeCollider::TestCollision(const Transform &transform, const CircleCollider *circle, const Transform &circleTransform) const 
    {
        return Algo::FindConvexShapeCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints ConvexShapeCollider::TestCollision(const Transform &transform, const ConvexShapeCollider *convexShape, const Transform &convexShapeTransform) const 
    {
        return Algo::FindConvexShapeConvexShapeCollision(this, transform, convexShape, convexShapeTransform);
    }

    CollisionPoints ConvexShapeCollider::TestCollision(const Transform &transform, const BoxCollider *box, const Transform &boxTransform) const
    {
        return Algo::FindConvexShapeBoxCollision(this, transform, box, boxTransform);
    }
#pragma endregion
}