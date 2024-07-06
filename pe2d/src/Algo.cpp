#include <cmath>
#include "Algo.hpp"
#include "Collider.hpp"

namespace pe2d
{
    CollisionPoints Algo::FindCircleCircleCollision(
        const CircleCollider *A, const Transform &transformA,
        const CircleCollider *B, const Transform &transformB)
    {
        Vector2 diff = transformA.position - transformB.position;
        
        const float &length = diff.magnitude();
        // I make assumption that it would be safest option in this situation but I don't know if any good solution is there
        // cuz I can only imagine how hard the math could be with calculating not circle sphere collision
        float sum = A->GetRadius() * transformA.scale.x + B->GetRadius() * transformB.scale.x;

        if(length > sum)
        {
            return CollisionPoints();
        }

        const float depth = sum - length;
        const Vector2 &normal = diff.normalized();
        
        return CollisionPoints{normal, depth, true};
    }

    CollisionPoints Algo::FindCircleSquareCollision(
        const CircleCollider *A, const Transform &transformA,
        const SquareCollider *B, const Transform &transformB)
    {
        const unsigned int verteciesCountB = B->GetVerteciesCount();
        float overlap = INF;
        const Vector2 &center = transformA.position;
        Vector2 verteciesB[verteciesCountB];
        GetSquareVertecies(verteciesB, verteciesCountB, transformB.position, B->GetSideLength(), transformB.scale, transformB.rotation);
        Vector2 axesB[verteciesCountB + 1];
        GetAxes(axesB, verteciesB, verteciesCountB);
        Vector2 *smallesAxis = nullptr;
        axesB[verteciesCountB - 1] = GetCircleAxis(verteciesB, verteciesCountB, center);


        for(int i = 0; i < verteciesCountB; i++)
        {
            Vector2 p1 = ProjectCircle(axesB[i], center, A->GetRadius() * transformA.scale.x);
            Vector2 p2 = Project(axesB[i], verteciesB, verteciesCountB);

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

    CollisionPoints Algo::FindSquareCircleCollision(
        const SquareCollider *A, const Transform &transformA,
        const CircleCollider *B, const Transform &transformB)
    {
        CollisionPoints p = FindCircleSquareCollision(B, transformB, A, transformA);
        p.Normal *= -1.0f;
        return p;
    }

    CollisionPoints Algo::FindSquareSquareCollision(
        const SquareCollider *A, const Transform &transformA,
        const SquareCollider *B, const Transform &transformB)
    {
        const unsigned int verteciesCountA = A->GetVerteciesCount();
        const unsigned int verteciesCountB = B->GetVerteciesCount();
        const float rotationA = transformA.GetRadians();
        const float rotationB = transformB.GetRadians();
        float overlap = INF;
        Vector2 verteciesA[verteciesCountA];
        Vector2 verteciesB[verteciesCountB];
        GetSquareVertecies( verteciesA, verteciesCountA, transformA.position, A->GetSideLength(), transformA.scale, rotationA );
        GetSquareVertecies( verteciesB, verteciesCountB, transformB.position, B->GetSideLength(), transformB.scale, rotationB );
        const Vector2 *smallesAxis = nullptr;
        Vector2 axesA[verteciesCountA];
        Vector2 axesB[verteciesCountB];
        GetAxes(axesA, verteciesA, verteciesCountA);
        GetAxes(axesB, verteciesB, verteciesCountB);

        for(const Vector2 &axis : axesA)
        {
            Vector2 p1 = Project(axis, verteciesA, verteciesCountA);
            Vector2 p2 = Project(axis, verteciesB, verteciesCountB);

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
            Vector2 p1 = Project(axis, verteciesA, verteciesCountA);
            Vector2 p2 = Project(axis, verteciesB, verteciesCountB);

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

    void Algo::GetAxes(Vector2 *axes, Vector2 *vertecies, int size)
    {
        for(int i = 0; i < size; i++)
        {
            Vector2 p1 = vertecies[i];
            Vector2 p2 = vertecies[(i + 1) % size];
            Vector2 edge = p2 - p1;
            Vector2 normal = edge.perp().normalized();
            axes[i] = normal;
        }
    }

    Vector2 Algo::Project(const Vector2 &axis, const Vector2 *vertecies, int size)
    {
        float min = INF;
        float max = -INF;

        for(int i = 0; i < size; i++)
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

    void Algo::RotateVertecies(Vector2 *vertecies, unsigned int size, const Vector2 &center, float angle)
    {
        const float cosAngle = cos(angle);
        const float sinAngle = sin(angle);
        for(int i = 0; i < size; i++)
        {
            const float translatedX = vertecies[i].x - center.x;
            const float translatedY = vertecies[i].y - center.y;

            const float rotatedX = translatedX * cosAngle - translatedY * sinAngle;
            const float rotatedY = translatedX * sinAngle + translatedY * cosAngle;
            vertecies[i] = Vector2{rotatedX + center.x, rotatedY + center.y};
        }
    }

    // only works for squares for now
    void Algo::GetSquareVertecies(Vector2 *vertecies, unsigned int size, const Vector2 &center, float sideLength, const Vector2 &scale, float angle)
    {
        vertecies[0] = Vector2{ center.x - sideLength / 2.0f * scale.x, center.y + sideLength / 2.0f * scale.y };
        vertecies[1] = Vector2{ center.x + sideLength / 2.0f * scale.x, center.y + sideLength / 2.0f * scale.y };
        vertecies[2] = Vector2{ center.x + sideLength / 2.0f * scale.x, center.y - sideLength / 2.0f * scale.y };
        vertecies[3] = Vector2{ center.x - sideLength / 2.0f * scale.x, center.y - sideLength / 2.0f * scale.y };
        RotateVertecies(vertecies, size, center, angle);
    }

    Vector2 Algo::GetCircleAxis(const Vector2 *vertecies, int size, const Vector2 &circleCenter)
    {
        float dist = INF;
        Vector2 smallestAxis = Vector2{};
        for(int i = 0; i < size; i++)
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