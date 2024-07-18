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
        float overlap = INF;
        const Vector2 &circleCenter = transformCircle.position;
        const float boxRotation = transformBox.GetRadians();
        Vector2 *smallesAxis = nullptr;
        const std::vector<Vector2> boxVertices = GetBoxVertices(box->GetSize(), transformBox);
        std::vector<Vector2> axes = GetRectangleAxes(boxVertices);
        axes.push_back(GetCircleAxis(boxVertices, circleCenter)) ;

        for(int i = 0; i < axes.size(); i++)
        {
            Vector2 p1 = ProjectCircle(axes[i], circleCenter, circle->GetRadius() * transformCircle.scale.x);
            Vector2 p2 = Project(boxVertices, axes[i]);

            if(!Overlap(p1, p2))
            {
                return CollisionPoints();
            }
            float o = GetOverlap(p1, p2);
            if(o < overlap)
            {
                overlap = o;
                smallesAxis = &axes[i];
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
        const Vector2 *smallestAxis = nullptr;
        float overlap = INF;
        const std::vector<Vector2> verticesA = GetBoxVertices(boxA->GetSize(), transformBoxA);
        const std::vector<Vector2> verticesB = GetBoxVertices(boxB->GetSize(), transformBoxB);
        const std::vector<Vector2> axesA = GetRectangleAxes(verticesA);
        const std::vector<Vector2> axesB = GetRectangleAxes(verticesB);

        for(int i = 0; i < axesA.size(); i++)
        {
            const Vector2 pA1 = Project(verticesA, axesA[i]);
            const Vector2 pA2 = Project(verticesB, axesA[i]);  
            
            if(!Overlap(pA1, pA2))
            {
                return CollisionPoints();
            }

            const Vector2 pB1 = Project(verticesA, axesB[i]);
            const Vector2 pB2 = Project(verticesB, axesB[i]);

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
        const float overlapStart = std::max(A.x, B.x);
        const float overlapEnd = std::min(A.y, B.y);

        return overlapEnd - overlapStart;
    }

    std::vector<Vector2> Algo::GetAxes(const std::vector<Vector2> &vertices)
    {
        std::vector<Vector2> axes;
        for(int i = 0; i < vertices.size(); i++)
        {
            const Vector2 p1 = vertices[i];
            const Vector2 p2 = vertices[(i + 1) % vertices.size()];
            const Vector2 edge = p1 - p2;
            const Vector2 normal = edge.perp().normalized();
            axes.push_back(normal);
        }
        return axes;
    }

    std::vector<Vector2> Algo::GetRectangleAxes(const std::vector<Vector2> &vertices)
    {
        std::vector<Vector2> axes;
        // is has two parrarel edges so I don't have to check other two
        for(int i = 0; i < vertices.size() / 2; i++)
        {
            const Vector2 p1 = vertices[i];
            const Vector2 p2 = vertices[(i + 1) % vertices.size()];
            const Vector2 edge = p1 - p2;
            const Vector2 normal = edge.perp().normalized();
            axes.push_back(normal);
        }
        return axes;
    }

    Vector2 Algo::Project(const std::vector<Vector2> &vertices, const Vector2 &axis)
    {
        float min = axis.dot(vertices[0]);
        float max = min;

        for(int i = 1; i < vertices.size(); i++)
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

    void Algo::RotateVertices(std::vector<Vector2> &vertices, const Vector2 &center, float angle)
    {
        const float cosAngle = cosf(angle);
        const float sinAngle = sinf(angle);
        for(int i = 0; i < vertices.size(); i++)
        {
            const float relativeX = vertices[i].x - center.x;
            const float relativeY = vertices[i].y - center.y;

            const float rotatedX = (relativeX * cosAngle) - (relativeY * sinAngle);
            const float rotatedY = (relativeX * sinAngle) + (relativeY * cosAngle);
            vertices[i] = Vector2{ rotatedX + center.x, rotatedY + center.y };
        }
    }
    
    std::vector<Vector2> Algo::GetBoxVertices(const Vector2 &boxSize, const Transform &transform)
    {
        const Vector2 &center = transform.position;
        const Vector2 &scale = transform.scale;
        const float scaledHalfSizeX = (boxSize.x * scale.x) / 2.0f;
        const float scaledHalfSizeY = (boxSize.y * scale.y) / 2.0f;
        const float angle = transform.GetRadians();
        std::vector<Vector2> vertices = 
        {
            Vector2{ center.x - scaledHalfSizeX, center.y + scaledHalfSizeY },
            Vector2{ center.x + scaledHalfSizeX, center.y + scaledHalfSizeY },
            Vector2{ center.x + scaledHalfSizeX, center.y - scaledHalfSizeY },
            Vector2{ center.x - scaledHalfSizeX, center.y - scaledHalfSizeY }
        };
        RotateVertices(vertices, center, angle);
        return vertices;
    }

    Vector2 Algo::GetCircleAxis(std::vector<Vector2> vertices, const Vector2 &circleCenter)
    {
        float dist = INF;
        Vector2 smallestAxis = Vector2{};
        for(int i = 0; i < vertices.size(); i++)
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