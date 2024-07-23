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
        return algo::FindCircleCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints CircleCollider::TestCollision(const Transform &transform, const BoxCollider *box, const Transform &boxTransform) const
    {
        return algo::FindCircleBoxCollision(this, transform, box, boxTransform);
    }

#pragma endregion

#pragma region Box Collider

    CollisionPoints BoxCollider::TestCollision(const Transform &transform, const Collider *collider, const Transform &colliderTransform) const
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }

    CollisionPoints BoxCollider::TestCollision(const Transform &transform, const CircleCollider *circle, const Transform &circleTransform) const
    {
        return algo::FindBoxCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints BoxCollider::TestCollision(const Transform &transform, const BoxCollider *box, const Transform &boxTransform) const
    {
        return algo::FindBoxBoxCollision(this, transform, box, boxTransform);
    }

#pragma endregion

}