#include "Collider.hpp"
#include "Algo.hpp"
namespace pe2d
{
#pragma region Circle Collider

    CollisionPoints CircleCollider::TestCollision(Transform transform, std::shared_ptr<Collider> collider, Transform colliderTransform)
    {
        return collider->TestCollision(colliderTransform, shared_from_this(), transform);
    }
    
    CollisionPoints CircleCollider::TestCollision(Transform transform, std::shared_ptr<CircleCollider> circle, Transform circleTransform)
    {
        return algo::FindCircleCircleCollision(shared_from_this(), transform, circle, circleTransform);
    }

    CollisionPoints CircleCollider::TestCollision(Transform transform, std::shared_ptr< BoxCollider> box, Transform boxTransform)
    {
        return algo::FindCircleBoxCollision(shared_from_this(), transform, box, boxTransform);
    }

#pragma endregion

#pragma region Box Collider

    CollisionPoints BoxCollider::TestCollision(Transform transform, std::shared_ptr<Collider> collider, Transform colliderTransform)
    {
        return collider->TestCollision(colliderTransform, shared_from_this(), transform);
    }

    CollisionPoints BoxCollider::TestCollision(Transform transform, std::shared_ptr<CircleCollider> circle, Transform circleTransform)
    {
        return algo::FindBoxCircleCollision(shared_from_this(), transform, circle, circleTransform);
    }

    CollisionPoints BoxCollider::TestCollision(Transform transform, std::shared_ptr<BoxCollider> box, Transform boxTransform)
    {
        return algo::FindBoxBoxCollision(shared_from_this(), transform, box, boxTransform);
    }

#pragma endregion

}