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

    CollisionPoints CircleCollider::TestCollision(const Transform &transform, const CustomCollider *custom, const Transform &customTransform) const 
    {
        return Algo::FindCircleCustomCollision(this, transform, custom, customTransform);
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

    CollisionPoints BoxCollider::TestCollision(const Transform &transform, const CustomCollider *custom, const Transform &customTransform) const
    {
        return Algo::FindBoxCustomCollision(this, transform, custom, customTransform);
    }

    CollisionPoints BoxCollider::TestCollision(const Transform &transform, const BoxCollider *box, const Transform &boxTransform) const
    {
        return Algo::FindBoxBoxCollision(this, transform, box, boxTransform);
    }

#pragma endregion

#pragma region Polygon Collider

    CollisionPoints CustomCollider::TestCollision( const Transform &transform, const Collider *collider, const Transform &colliderTransform) const 
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }

    CollisionPoints CustomCollider::TestCollision(const Transform &transform, const CircleCollider *circle, const Transform &circleTransform) const 
    {
        return Algo::FindCustomCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints CustomCollider::TestCollision(const Transform &transform, const CustomCollider *custom, const Transform &customTransform) const 
    {
        return Algo::FindCustomCustomCollision(this, transform, custom, customTransform);
    }

    CollisionPoints CustomCollider::TestCollision(const Transform &transform, const BoxCollider *box, const Transform &boxTransform) const
    {
        return Algo::FindCustomBoxCollision(this, transform, box, boxTransform);
    }
#pragma endregion
}