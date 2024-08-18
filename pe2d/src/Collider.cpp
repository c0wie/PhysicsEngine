#include "Collider.hpp"
#include "Algo.hpp"
namespace pe2d
{
#pragma region Circle Collider

    CollisionPoints CircleCollider::TestCollision(Transform transform, const Collider *collider, Transform colliderTransform) const 
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }
    
    CollisionPoints CircleCollider::TestCollision(Transform transform, const CircleCollider *circle, Transform circleTransform) const 
    {
        return algo::FindCircleCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints CircleCollider::TestCollision(Transform transform, const BoxCollider *box, Transform boxTransform) const
    {
        return algo::FindCircleBoxCollision(this, transform, box, boxTransform);
    }

#pragma endregion

#pragma region Box Collider

    CollisionPoints BoxCollider::TestCollision(Transform transform, const Collider *collider, Transform colliderTransform) const
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }

    CollisionPoints BoxCollider::TestCollision(Transform transform, const CircleCollider *circle, Transform circleTransform) const
    {
        return algo::FindBoxCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints BoxCollider::TestCollision(Transform transform, const BoxCollider *box, Transform boxTransform) const
    {
        return algo::FindBoxBoxCollision(this, transform, box, boxTransform);
    }

#pragma endregion

}