#pragma once

#include <array>
#include <memory>
#include <limits>
#include "CollisionPoints.hpp"
#include "Transform.hpp"

namespace pe2d
{
    class CircleCollider;
    class CustomCollider;
    class BoxCollider;

    class Algo
    {
    public:
        static CollisionPoints FindCircleCircleCollision(
            const CircleCollider *circleA, const Transform &transformCircleA,
            const CircleCollider *circleB, const Transform &transformCircleB);
    
        static CollisionPoints FindCircleBoxCollision(
            const CircleCollider *circle, const Transform &transformCircle,
            const BoxCollider *box, const Transform &transformBox);
    
        static CollisionPoints FindBoxCircleCollision(
            const BoxCollider *box, const Transform &transformBox,
            const CircleCollider *circle, const Transform &transformCircle);

        static CollisionPoints FindCircleCustomCollision(
            const CircleCollider *circle, const Transform &transformCircle,
            const CustomCollider *custom, const Transform &transformCustom);
        
        static CollisionPoints FindCustomCircleCollision(
            const CustomCollider *custom, const Transform &transformCustom, 
            const CircleCollider *circle, const Transform &transformCircle);

        static CollisionPoints FindCustomCustomCollision(
            const CustomCollider *customA, const Transform &transformCustomA,
            const CustomCollider *customB, const Transform &transformCustomB);
        
        static CollisionPoints FindCustomBoxCollision(
            const CustomCollider *custom, const Transform &transformCustom,
            const BoxCollider *box, const Transform &transformBox);

        static CollisionPoints FindBoxCustomCollision(
            const BoxCollider *box, const Transform &transformBox, 
            const CustomCollider *custom, const Transform &transformCustom);

        static CollisionPoints FindBoxBoxCollision(
            const BoxCollider *boxA, const Transform &transformBoxA,
            const BoxCollider *boxB, const Transform &transformBoxB);
    private:
        static bool Overlap(const Vector2 &A, const Vector2 &B);
        static float GetOverlap(const Vector2 &A, const Vector2 &B);
        static void GetAxes(Vector2 *const axes, const Vector2 *const vertecies, unsigned int count);
        static Vector2 Project(const Vector2 *const vertecies, unsigned int count, const Vector2 &axis);
        static Vector2 ProjectCircle(const Vector2 &axis, const Vector2 &circleCenter, float radius);
        static void RotateVertecies(Vector2 *const vertecies, unsigned int count, const Vector2 &center, float angle);
        static void GetBoxVertecies(Vector2 *const vertecies, unsigned int count, const Vector2 &center, const Vector2 &boxSize,
                                    const Vector2 &scale, float angle);
        static void GetCustomVertecies(Vector2 *const vertecies, unsigned int count, const Vector2 *offsets, const Vector2 &center);
        static Vector2 GetCircleAxis(const Vector2 *const vertecies, unsigned int count, const Vector2 &circleCenter);
    private:
        static constexpr float INF = std::numeric_limits<float>::infinity();
    };
}