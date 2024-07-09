#include <cmath>
#include "Algo.hpp"
#include "Collider.hpp"

namespace pe2d
{
    CollisionPoints Algo::FindCircleCircleCollision(
        const CircleCollider *circleA, const Transform &transformCircleA,
        const CircleCollider *circleB, const Transform &transformCircleB)
    {
        Vector2 diff = transformCircleA.position - transformCircleB.position;
        
        const float &length = diff.magnitude();
        // I make assumption that it would be safest option in this situation but I don't know if any good solution is there
        // cuz I can only imagine how hard the math could be with calculating not circle sphere collision
        float sum = circleA->GetRadius() * transformCircleA.scale.x + circleB->GetRadius() * transformCircleB.scale.x;

        if(length > sum)
        {
            return CollisionPoints();
        }

        const float depth = sum - length;
        const Vector2 &normal = diff.normalized();
        
        return CollisionPoints{normal, depth, true};
    }

    CollisionPoints Algo::FindCircleBoxCollision(
        const CircleCollider *circle, const Transform &transformCircle,
        const BoxCollider *box, const Transform &transformBox)
    {
        const unsigned int boxVerteciesCount = 4;
        float overlap = INF;
        const Vector2 &center = transformCircle.position;
        Vector2 *smallesAxis = nullptr;

        Vector2 verteciesB[boxVerteciesCount];
        GetBoxVertecies(verteciesB, boxVerteciesCount, transformBox.position, box->GetSize(), transformBox.scale, transformBox.rotation);
        Vector2 axesB[boxVerteciesCount + 1];
        GetAxes(axesB, verteciesB, boxVerteciesCount);
        axesB[boxVerteciesCount - 1] = GetCircleAxis(verteciesB, boxVerteciesCount, center);

        for(int i = 0; i < boxVerteciesCount; i++)
        {
            Vector2 p1 = ProjectCircle(axesB[i], center, circle->GetRadius() * transformCircle.scale.x);
            Vector2 p2 = Project(verteciesB, boxVerteciesCount, axesB[i]);

            if(!Overlap(p1, p2))
            {
                return CollisionPoints();
            }
            float o = GetOverlap(p1, p2);
            if(o < overlap)
            {
                overlap = o;
                smallesAxis = &axesB[i];
            }
        }
        return CollisionPoints{*smallesAxis, overlap, true};
    }

    CollisionPoints Algo::FindBoxCircleCollision(
        const BoxCollider *box, const Transform &transformBox,
        const CircleCollider *circle, const Transform &transformCircle)
    {
        CollisionPoints p = FindCircleBoxCollision(circle, transformCircle, box, transformBox);
        p.Normal *= -1.0f;
        return p;
    }
    
    CollisionPoints Algo::FindCircleConvexShapeCollision(
        const CircleCollider *circle, const Transform &transformCircle,
        const ConvexShapeCollider *convexShape, const Transform &transformConvexShape)
    {
        const unsigned int convexShapeVerteciesCount = convexShape->GetVerteciesCount();
        float overlap = INF;
        const Vector2 &center = transformCircle.position;
        Vector2 *smallesAxis = nullptr;

        Vector2 *verteciesB = convexShape->GetVertecies();
        for(int i = 0; i < convexShapeVerteciesCount; i ++) { verteciesB[i] += transformConvexShape.position; }
        Vector2 axesB[convexShapeVerteciesCount + 1];
        GetAxes(axesB, verteciesB, convexShapeVerteciesCount);
        axesB[convexShapeVerteciesCount - 1] = GetCircleAxis(verteciesB, convexShapeVerteciesCount, center);


        for(int i = 0; i < convexShapeVerteciesCount; i++)
        {
            Vector2 p1 = ProjectCircle(axesB[i], center, circle->GetRadius() * transformCircle.scale.x);
            Vector2 p2 = Project(verteciesB, convexShapeVerteciesCount, axesB[i]);

            if(!Overlap(p1, p2))
            {
                return CollisionPoints();
            }
            float o = GetOverlap(p1, p2);
            if(o < overlap)
            {
                overlap = o;
                smallesAxis = &axesB[i];
            }
        }
        return CollisionPoints{*smallesAxis, overlap, true};
    }

    CollisionPoints Algo::FindConvexShapeCircleCollision(
        const ConvexShapeCollider *convexShape, const Transform &transformConvexShape,
        const CircleCollider *circle, const Transform &transformCircle)
    {
        CollisionPoints p = FindCircleConvexShapeCollision(circle, transformCircle, convexShape, transformConvexShape);
        p.Normal *= -1.0f;
        return p;
    }

    CollisionPoints Algo::FindConvexShapeConvexShapeCollision(
        const ConvexShapeCollider *convexShapeA, const Transform &transformConvexShapeA,
        const ConvexShapeCollider *convexShapeB, const Transform &transformConvexShapeB)
    {
        const unsigned int verteciesCountA = convexShapeA->GetVerteciesCount();
        const unsigned int verteciesCountB = convexShapeB->GetVerteciesCount();
        const float rotationA = transformConvexShapeA.GetRadians();
        const float rotationB = transformConvexShapeB.GetRadians();
        const Vector2 *smallesAxis = nullptr;
        float overlap = INF;

        Vector2 *verteciesA = convexShapeA->GetVertecies();
        Vector2 *verteciesB = convexShapeB->GetVertecies();
        Vector2 axesA[verteciesCountA];
        Vector2 axesB[verteciesCountB];
        GetAxes(axesA, verteciesA, verteciesCountA);
        GetAxes(axesB, verteciesB, verteciesCountB);

        for(const Vector2 &axis : axesA)
        {
            Vector2 p1 = Project(verteciesA, verteciesCountA, axis);
            Vector2 p2 = Project(verteciesB, verteciesCountB, axis);

            if(!Overlap(p1, p2))
            {
                return CollisionPoints();
            }
            float o = GetOverlap(p1, p2);
            if(o < overlap)
            {
                overlap = o;
                smallesAxis = &axis;
            }
        }

        for(const Vector2 &axis : axesB)
        {
            Vector2 p1 = Project(verteciesA, verteciesCountA, axis);
            Vector2 p2 = Project(verteciesB, verteciesCountB, axis);

            if(!Overlap(p1, p2))
            {
                return CollisionPoints();
            }
            float o = GetOverlap(p1, p2);
            if(o < overlap)
            {
                overlap = o;
                smallesAxis = &axis;
            }
        }
        return CollisionPoints{*smallesAxis, overlap, true};
    }

    CollisionPoints Algo::FindConvexShapeBoxCollision(
        const ConvexShapeCollider *convexShape, const Transform &transformConvexShape,
        const BoxCollider *box, const Transform &transformBox)
    {
        const unsigned int convexShapeVerteciesCount = convexShape->GetVerteciesCount();
        const unsigned int boxVerteciesCount = 4;
        const float ConvexShapeRotation = transformConvexShape.GetRadians();
        const float boxRotation = transformBox.GetRadians();
        const Vector2 *smallesAxis = nullptr;
        float overlap = INF;

        Vector2 *ConvexShapeVertecies = convexShape->GetVertecies();
        Vector2 boxVerecies[boxVerteciesCount];
        GetBoxVertecies(boxVerecies, boxVerteciesCount, transformBox.position, box->GetSize(), transformBox.scale, boxRotation);

        Vector2 ConvexShapeAxes[convexShapeVerteciesCount];
        Vector2 boxAxes[boxVerteciesCount];
        GetAxes(ConvexShapeAxes, ConvexShapeVertecies, convexShapeVerteciesCount);
        GetAxes(boxAxes, boxVerecies, boxVerteciesCount);

        for(const Vector2 &axis : ConvexShapeAxes)
        {
            Vector2 p1 = Project(ConvexShapeVertecies, convexShapeVerteciesCount, axis);
            Vector2 p2 = Project(boxVerecies, boxVerteciesCount, axis);

            if(!Overlap(p1, p2))
            {
                return CollisionPoints();
            }
            float o = GetOverlap(p1, p2);
            if(o < overlap)
            {
                overlap = o;
                smallesAxis = &axis;
            }
        }

        for(const Vector2 &axis : boxAxes)
        {
            Vector2 p1 = Project(ConvexShapeVertecies, convexShapeVerteciesCount, axis);
            Vector2 p2 = Project(boxVerecies, boxVerteciesCount, axis);

            if(!Overlap(p1, p2))
            {
                return CollisionPoints();
            }
            float o = GetOverlap(p1, p2);
            if(o < overlap)
            {
                overlap = o;
                smallesAxis = &axis;
            }
        }
        return CollisionPoints{*smallesAxis, overlap, true};
    }

    CollisionPoints Algo::FindBoxConvexShapeCollision(
        const BoxCollider *box, const Transform &transformBox, 
        const ConvexShapeCollider *convexShape, const Transform &transformConvexShape)
    {
        CollisionPoints p = FindConvexShapeBoxCollision(convexShape, transformConvexShape, box, transformBox);
        p.Normal *= -1.0f;
        return p;
    }

    CollisionPoints Algo::FindBoxBoxCollision(
        const BoxCollider *boxA, const Transform &transformBoxA,
        const BoxCollider *boxB, const Transform &transformBoxB)
    {
        constexpr unsigned int verteciesCount = 4;
        const float boxARotation = transformBoxA.GetRadians();
        const float boxBRotation = transformBoxB.GetRadians();
        const Vector2 *smallesAxis = nullptr;
        float overlap = INF;

        Vector2 boxAVertecies[verteciesCount];
        Vector2 boxBVertecies[verteciesCount];
        GetBoxVertecies(boxAVertecies, verteciesCount, transformBoxA.position, boxA->GetSize(), transformBoxA.scale, boxARotation);
        GetBoxVertecies(boxBVertecies, verteciesCount, transformBoxB.position, boxB->GetSize(), transformBoxB.scale, boxBRotation);

        Vector2 boxAAxes[verteciesCount];
        Vector2 boxBAxes[verteciesCount];
        GetAxes(boxAAxes, boxAVertecies, verteciesCount);
        GetAxes(boxBAxes, boxBVertecies, verteciesCount);

        for(int i = 0; i < verteciesCount; i++)
        {
            const Vector2 pA1 = Project(boxAVertecies, verteciesCount, boxAAxes[i]);
            const Vector2 pA2 = Project(boxBVertecies, verteciesCount, boxAAxes[i]);  

            const Vector2 pB1 = Project(boxAVertecies, verteciesCount, boxBAxes[i]);
            const Vector2 pB2 = Project(boxBVertecies, verteciesCount, boxBAxes[i]);

            if(!Overlap(pA1, pA2) || !Overlap(pB1, pB2))
            {
                return CollisionPoints();
            }
            const float overlapA = GetOverlap(pA1, pA2);
            const float overlapB = GetOverlap(pB1, pB2);
            if(overlapA < overlap)
            {
                overlap = overlapA;
                smallesAxis = &boxAAxes[i];
            }
            else if(overlapB < overlap)
            {
                overlap = overlapB;
                smallesAxis = &boxBAxes[i];
            }
        }
        return CollisionPoints{*smallesAxis, overlap, true};
    }

    bool Algo::Overlap(const Vector2 &A, const Vector2 &B)
    {
        if(A.y >= B.x && B.y >= A.x)
        {
            return true;
        }
        return false;
    }

    float Algo::GetOverlap(const Vector2 &A, const Vector2 &B)
    {
        float overlapStart = std::max(A.x, B.x);
        float overlapEnd = std::min(A.y, B.y);

        return overlapEnd - overlapStart;
    }

    void Algo::GetAxes(Vector2 *const axes, const Vector2 *const vertecies, unsigned int count)
    {
        for(int i = 0; i < count; i++)
        {
            Vector2 p1 = vertecies[i];
            Vector2 p2 = vertecies[(i + 1) % count];
            Vector2 edge = p2 - p1;
            Vector2 normal = edge.perp().normalized();
            axes[i] = normal;
        }
    }

    Vector2 Algo::Project(const Vector2 *const vertecies, unsigned int count, const Vector2 &axis)
    {
        float min = INF;
        float max = -INF;

        for(int i = 0; i < count; i++)
        {
            float p = axis.dot(vertecies[i]);
            if(p < min)
            {
                min = p;
            }
            else if(p > max)
            {
                max = p;
            }
        }
        return Vector2{min, max};
    }    
    
    Vector2 Algo::ProjectCircle(const Vector2 &axis, const Vector2 &circleCenter, float radius)
    {
        const Vector2 dir = axis * radius;

        const Vector2 p1 = circleCenter + dir;
        const Vector2 p2 = circleCenter - dir;

        float min = p1.dot(axis);
        float max = p2.dot(axis);

        if(min > max)
        {
            std::swap(min, max);
        } 
        return Vector2{min, max};
    }

    void Algo::RotateVertecies(Vector2 *const vertecies, unsigned int count, const Vector2 &center, float angle)
    {
        const float cosAngle = cos(angle);
        const float sinAngle = sin(angle);
        for(int i = 0; i < count; i++)
        {
            const float translatedX = vertecies[i].x - center.x;
            const float translatedY = vertecies[i].y - center.y;

            const float rotatedX = translatedX * cosAngle - translatedY * sinAngle;
            const float rotatedY = translatedX * sinAngle + translatedY * cosAngle;
            vertecies[i] = Vector2{rotatedX + center.x, rotatedY + center.y};
        }
    }
    
    void Algo::GetBoxVertecies(Vector2 *const vertecies, unsigned int count, const Vector2 &center, const Vector2 &boxSize,
                                const Vector2 &scale, float angle)
    {
        vertecies[0] = Vector2{ center.x - boxSize.x / 2.0f * scale.x, center.y + boxSize.y / 2.0f * scale.y };
        vertecies[1] = Vector2{ center.x + boxSize.x / 2.0f * scale.x, center.y + boxSize.y / 2.0f * scale.y };
        vertecies[2] = Vector2{ center.x + boxSize.x / 2.0f * scale.x, center.y - boxSize.y / 2.0f * scale.y };
        vertecies[3] = Vector2{ center.x - boxSize.x / 2.0f * scale.x, center.y - boxSize.y / 2.0f * scale.y };
        RotateVertecies(vertecies, count, center, angle);
    }

    void Algo::GetConvexShapeVertecies(Vector2 *const vertecies, unsigned int count, const Vector2 *offsets, const Vector2 &center)
    {
        for(int i = 0; i < count; i++)
        {
            vertecies[i] = center + offsets[i];
        }
    }

    Vector2 Algo::GetCircleAxis(const Vector2 *const vertecies, unsigned int count, const Vector2 &circleCenter)
    {
        float dist = INF;
        Vector2 smallestAxis = Vector2{};
        for(int i = 0; i < count; i++)
        {
            Vector2 edge = vertecies[i] - circleCenter;
            float d = edge.magnitude();
            if(d < dist)
            {
                dist = d;
                smallestAxis = edge;
            }
        }
        return smallestAxis.normalized();
    }
}