#include <cmath>
#include "Algo.hpp"
#include "Collider.hpp"

namespace phy
{
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

    std::vector<Vector2> Algo::GetAxes(const std::vector<Vector2> &vertecies)
    {
        std::vector<Vector2> axes;
        for(int i = 0; i < vertecies.size(); i++)
        {
            Vector2 p1 = vertecies[i];
            Vector2 p2 = vertecies[(i + 1) % vertecies.size()];
            Vector2 edge = p2 - p1;
            Vector2 normal = edge.perp().normalized();
            axes.push_back(normal);
        }
        return axes;
    }

    Vector2 Algo::Project(const Vector2 &axis, const std::vector<Vector2> &vertecies)
    {
        float min = INF;
        float max = -INF;

        for(int i = 0; i < vertecies.size(); i++)
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

    std::vector<Vector2> Algo::GetSquareVertecies(const Vector2 &center, float sideLength, const Vector2 &scale)
    {
        const std::vector<Vector2> vertecies = 
        {
            Vector2{ center.x - sideLength / 2.0f * scale.x, center.y + sideLength / 2.0f * scale.y },
            Vector2{ center.x + sideLength / 2.0f * scale.x, center.y + sideLength / 2.0f * scale.y },
            Vector2{ center.x + sideLength / 2.0f * scale.x, center.y - sideLength / 2.0f * scale.y },
            Vector2{ center.x - sideLength / 2.0f * scale.x, center.y - sideLength / 2.0f * scale.y }
        };
        return vertecies;
    }

    Vector2 Algo::GetCircleAxis(const std::vector<Vector2> vertecies, const Vector2 &circleCenter)
    {
        float dist = INF;
        Vector2 smallestAxis = Vector2{};
        for(int i = 0; i < vertecies.size(); i++)
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

    Vector2 Algo::FindFurthestPoint(const std::vector<Vector2> &vertecies, const Vector2 &direction)
    {
        float maxProjection = -INF;
        const Vector2 *furthestPoint = nullptr;
        for(int i = 0; i < 4; i++)
        {
            float projection = vertecies[i].dot(direction);
            if(projection > maxProjection)
            {
                maxProjection = projection;
                furthestPoint = &vertecies[i];
            }
        }  
        return *furthestPoint;
    }

    CollisionPoints Algo::FindCircleCircleCollision(
        const CircleCollider *A, const Transform *transformA,
        const CircleCollider *B, const Transform *transformB)
    {
        Vector2 diff = transformA->position - transformB->position;
        
        const float &length = diff.magnitude();
        // I make assumption that it would be safest option in this situation but I don't know if any good solution is there
        // cuz I can only imagine how hard the math could be with calculating not circle sphere collision
        float sum = A->GetRadius() * transformA->scale.x + B->GetRadius() * transformB->scale.x;

        if(length >= sum)
        {
            return CollisionPoints();
        }

        const float depth = sum - length;
        const Vector2 &normal = diff.normalized();
        
        return CollisionPoints{normal, depth, true};
    }

    CollisionPoints Algo::FindCircleSquareCollision(
        const CircleCollider *A, const Transform *transformA,
        const SquareCollider *B, const Transform *transformB)
    {
        float overlap = INF;
        const Vector2 &center = transformA->position;
        const std::vector<Vector2> verteciesB = GetSquareVertecies(transformB->position, B->GetSideLength(), transformB->scale);
        std::vector<Vector2> axesB = GetAxes(verteciesB);
        Vector2 *smallesAxis = nullptr;
        axesB.push_back(GetCircleAxis(verteciesB, center));


        for(int i = 0; i < axesB.size(); i++)
        {
            Vector2 p1 = ProjectCircle(axesB[i], center, A->GetRadius() * transformA->scale.x);
            Vector2 p2 = Project(axesB[i], verteciesB);

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
        LogCall("Circle - Square collision\n");
        return CollisionPoints{*smallesAxis, overlap, true};
    }

    CollisionPoints Algo::FindSquareCircleCollision(
        const SquareCollider *A, const Transform *transformA,
        const CircleCollider *B, const Transform *transformB)
    {
        CollisionPoints p = FindCircleSquareCollision(B, transformB, A, transformA);
        p.Normal *= -1.0f;
        return p;
    }

    CollisionPoints Algo::FindSquareSquareCollision(
        const SquareCollider *A, const Transform *transformA,
        const SquareCollider *B, const Transform *transformB)
    {
        float overlap = INF;
        const std::vector<Vector2> verteciesA = GetSquareVertecies(transformA->position, A->GetSideLength(), transformA->scale);
        const std::vector<Vector2> verteciesB = GetSquareVertecies(transformB->position, B->GetSideLength(), transformB->scale);
        const Vector2 *smallesAxis = nullptr;
        const std::vector<Vector2> axesA = GetAxes(verteciesA);
        const std::vector<Vector2> axesB = GetAxes(verteciesB);
        for(int i = 0; i < axesA.size(); i++)
        {
            Vector2 p1 = Project(axesA[i], verteciesA);
            Vector2 p2 = Project(axesA[i], verteciesB);

            if(!Overlap(p1, p2))
            {
                return CollisionPoints();
            }
            float o = GetOverlap(p1, p2);
            if(o < overlap)
            {
                overlap = o;
                smallesAxis = &axesA[i];
            }
        }

        for(int i = 0; i < axesB.size(); i++)
        {
            Vector2 p1 = Project(axesB[i], verteciesA);
            Vector2 p2 = Project(axesB[i], verteciesB);

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
}