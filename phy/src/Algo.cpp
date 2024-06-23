#include <cmath>
#include "../headers/Algo.hpp"
#include "../headers/CircleCollider.hpp"
#include "../headers/SquareCollider.hpp"

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
        float min = axis.dot(vertecies[0]);
        float max = min;

        for(int i = 1; i < vertecies.size(); i++)
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
    
    std::vector<Vector2> Algo::GetSquareVertecies(const Vector2 &center, float sideLength)
    {
        const std::vector<Vector2> vertecies = 
        {
            Vector2{center.x - sideLength / 2.0f, center.y + sideLength / 2.0f},
            Vector2{center.x + sideLength / 2.0f, center.y + sideLength / 2.0f},
            Vector2{center.x + sideLength / 2.0f, center.y - sideLength / 2.0f},
            Vector2{center.x - sideLength / 2.0f, center.y - sideLength / 2.0f}
        };
        return vertecies;
    }

    Vector2 Algo::FindFurthestPoint(const std::vector<Vector2> &vertecies, const Vector2 &direction)
    {
        float maxProjection = (float) INT_MIN;
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
        const Vector2 cA = transformA->Position;
        const Vector2 cB = transformB->Position;
        const Vector2 v = cB - cA;
        
        const float magnitude = sqrt(pow(v.x, 2) + pow(v.y, 2));
        if(magnitude >= A->Radius + B->Radius)
        {
            return CollisionPoints();
        }
        const Vector2 normal = v / magnitude;
        const Vector2 pointA = transformA->Position - normal * A->Radius;
        const Vector2 pointB = transformB->Position + normal * B->Radius;
        
        return CollisionPoints{pointA, pointB, normal, magnitude, true};
    }

    CollisionPoints Algo::FindCircleSquareCollision(
        const CircleCollider *A, const Transform *transformA,
        const SquareCollider *B, const Transform *transformB)
    {
        return CollisionPoints();
    }

    CollisionPoints Algo::FindSquareCircleCollision(
        const SquareCollider *A, const Transform *transformA,
        const CircleCollider *B, const Transform *transformB)
    {
        return CollisionPoints();
    }

    CollisionPoints Algo::FindSquareSquareCollision(
        const SquareCollider *A, const Transform *transformA,
        const SquareCollider *B, const Transform *transformB)
    {
        const  std::vector<Vector2> verteciesA = GetSquareVertecies(transformA->Position, A->SideLength);
        const std::vector<Vector2> verteciesB = GetSquareVertecies(transformB->Position, B->SideLength);
        float overlap = (float)INT_MAX;
        const Vector2 *smallesAxis = nullptr;
        // size = 4 cuz there are only squares for now
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
        const Vector2 furthestPointA = FindFurthestPoint(verteciesA, *smallesAxis);
        const Vector2 furthestPointB = FindFurthestPoint(verteciesB, *smallesAxis);

        return CollisionPoints{furthestPointA, furthestPointB, *smallesAxis, overlap, true};
    }
}