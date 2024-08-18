#include "Collider.hpp"
#include "Algo.hpp"
namespace pe2d
{
#pragma region Circle Collider

    bool CircleCollider::TestCollision(Transform transform, const Collider *collider, Transform colliderTransform, Vector2 &normal, float &depth) const 
    {
        return collider->TestCollision(colliderTransform, this, transform, normal, depth);
    }
    
    bool CircleCollider::TestCollision(Transform transform, const CircleCollider *circle, Transform circleTransform, Vector2 &normal, float &depth) const 
    {
        return algo::FindCircleCircleCollision(this, transform, circle, circleTransform, normal, depth);
    }

    bool CircleCollider::TestCollision(Transform transform, const BoxCollider *box, Transform boxTransform, Vector2 &normal, float &depth) const
    {
        return algo::FindCircleBoxCollision(this, transform, box, boxTransform, normal, depth);
    }

#pragma endregion

#pragma region Box Collider

    bool BoxCollider::TestCollision(Transform transform, const Collider *collider, Transform colliderTransform, Vector2 &normal, float &depth) const
    {
        return collider->TestCollision(colliderTransform, this, transform, normal, depth);
    }

    bool BoxCollider::TestCollision(Transform transform, const CircleCollider *circle, Transform circleTransform, Vector2 &normal, float &depth) const
    {
        return algo::FindBoxCircleCollision(this, transform, circle, circleTransform, normal, depth);
    }

    bool BoxCollider::TestCollision(Transform transform, const BoxCollider *box, Transform boxTransform, Vector2 &normal, float &depth) const
    {
        return algo::FindBoxBoxCollision(this, transform, box, boxTransform, normal, depth);
    }

#pragma endregion

}