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
        constexpr unsigned int boxverticesCount = 4;
        constexpr unsigned int axesCount = boxverticesCount + 1;
        float overlap = INF;
        const Vector2 &center = transformCircle.position;
        Vector2 *smallesAxis = nullptr;
        Vector2 verticesB[boxverticesCount];
        Vector2 axesB[axesCount];

        GetBoxVertices(verticesB, boxverticesCount, transformBox.position, box->GetSize(), transformBox.scale, transformBox.rotation);
        GetAxes(axesB, verticesB, boxverticesCount);
        axesB[axesCount - 1] = GetCircleAxis(verticesB, boxverticesCount, center);

        for(int i = 0; i < axesCount; i++)
        {
            Vector2 p1 = ProjectCircle(axesB[i], center, circle->GetRadius() * transformCircle.scale.x);
            Vector2 p2 = Project(verticesB, boxverticesCount, axesB[i]);

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
    
    CollisionPoints Algo::FindBoxBoxCollision(
        const BoxCollider *boxA, const Transform &transformBoxA,
        const BoxCollider *boxB, const Transform &transformBoxB)
    {
        constexpr unsigned int verticesCount = 4;
        // skip same axis with minus
        constexpr unsigned int axesCount = verticesCount;
        const float RotationA = transformBoxA.GetRadians();
        const float RotationB = transformBoxB.GetRadians();
        const Vector2 *smallestAxis = nullptr;
        float overlap = INF;
        Vector2 verticesA[verticesCount];
        Vector2 verticesB[verticesCount];
        Vector2 axesA[verticesCount];
        Vector2 axesB[verticesCount];

        GetBoxVertices(verticesA, verticesCount, transformBoxA.position, boxA->GetSize(), transformBoxA.scale, RotationA);
        GetBoxVertices(verticesB, verticesCount, transformBoxB.position, boxB->GetSize(), transformBoxB.scale, RotationB);
        GetAxes(axesA, verticesA, axesCount);
        GetAxes(axesB, verticesB, axesCount);

        for(int i = 0; i < axesCount; i++)
        {
            const Vector2 pA1 = Project(verticesA, verticesCount, axesA[i]);
            const Vector2 pA2 = Project(verticesB, verticesCount, axesA[i]);  
            
            if(!Overlap(pA1, pA2))
            {
                return CollisionPoints();
            }

            const Vector2 pB1 = Project(verticesA, verticesCount, axesB[i]);
            const Vector2 pB2 = Project(verticesB, verticesCount, axesB[i]);

            if(!Overlap(pB1, pB2))
            {
                return CollisionPoints();
            }

            const float overlapA = GetOverlap(pA1, pA2);
            const float overlapB = GetOverlap(pB1, pB2);

            if(overlapA < overlap)
            {
                overlap = overlapA;
                smallestAxis = &axesA[i];
            }
            if(overlapB < overlap)
            {
                overlap = overlapB;
                smallestAxis = &axesB[i];
            }
        }

        return CollisionPoints{*smallestAxis, overlap, true};
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

    void Algo::GetAxes(Vector2 *const axes, const Vector2 *const vertices, unsigned int count)
    {
        for(int i = 0; i < count; i++)
        {
            const Vector2 p1 = vertices[i];
            const Vector2 p2 = vertices[(i + 1) % count];
            const Vector2 edge = p2 - p1;
            const Vector2 normal = edge.perp().normalized();
            axes[i] = normal;            
        }
    }

    Vector2 Algo::Project(const Vector2 *const vertices, unsigned int count, const Vector2 &axis)
    {
        float min = INF;
        float max = -INF;

        for(int i = 0; i < count; i++)
        {
            const float p = axis.dot(vertices[i]);
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

    void Algo::RotateVertices(Vector2 *const vertices, unsigned int count, const Vector2 &center, float angle)
    {
        const float cosAngle = cosf(angle);
        const float sinAngle = sinf(angle);
        for(int i = 0; i < count; i++)
        {
            Vector2 &vertex = vertices[i];
            const float relativeX = vertex.x - center.x;
            const float relativeY = vertex.y - center.y;

            const float rotatedX = (relativeX * cosAngle) - (relativeY * sinAngle);
            const float rotatedY = (relativeX * sinAngle) + (relativeY * cosAngle);
            vertex = Vector2{ rotatedX + center.x, rotatedY + center.y };
        }
    }
    
    void Algo::GetBoxVertices(Vector2 *const vertices, unsigned int count, const Vector2 &center, const Vector2 &boxSize, const Vector2 &scale, float angle)
    {
        const float scaledHalfSizeX = (boxSize.x * scale.x) / 2.0f;
        const float scaledHalfSizeY = (boxSize.y * scale.y) / 2.0f ;
        vertices[0] = Vector2{ center.x - scaledHalfSizeX, center.y + scaledHalfSizeY };
        vertices[1] = Vector2{ center.x + scaledHalfSizeX, center.y + scaledHalfSizeY };
        vertices[2] = Vector2{ center.x + scaledHalfSizeX, center.y - scaledHalfSizeY };
        vertices[3] = Vector2{ center.x - scaledHalfSizeX, center.y - scaledHalfSizeY };
        RotateVertices(vertices, count, center, angle);
    }

    Vector2 Algo::GetCircleAxis(const Vector2 *const vertices, unsigned int count, const Vector2 &circleCenter)
    {
        float dist = INF;
        Vector2 smallestAxis = Vector2{};
        for(int i = 0; i < count; i++)
        {
            Vector2 edge = vertices[i] - circleCenter;
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