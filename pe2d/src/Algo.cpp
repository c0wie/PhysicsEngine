#include <cmath>
#include "Algo.hpp"
#include "Collider.hpp"

namespace pe2d
{
    namespace algo
    {
        CollisionPoints FindCircleCircleCollision(
            const CircleCollider *circleA, Transform transformCircleA,
            const CircleCollider *circleB, Transform transformCircleB)
        {
            const float radiusA = circleA->GetRadius();
            const float radiusB =  circleB->GetRadius();
            const Vector2 diff = transformCircleA.position - transformCircleB.position;
            const float &length = diff.magnitude();
            if(length == 0.0f)
            {
                const float overlap = radiusA > radiusB? radiusA : radiusB;
                return CollisionPoints{pe2d::Vector2{1.0f, 0.0f}, overlap, true};
            }
            // with circle I scaled them based on x value of scale
            float sum = ( radiusA * transformCircleA.scale.x ) + ( radiusB * transformCircleB.scale.x );

            if(length > sum)
            {
                return CollisionPoints();
            }

            const float overlap = sum - length;
            Vector2 normal = diff.normalized();

            return CollisionPoints{normal, overlap, true};
        }

        CollisionPoints FindCircleBoxCollision(
            const CircleCollider *circle, Transform transformCircle,
            const BoxCollider *box, Transform transformBox)
        {
            Vector2 circleCenter = transformCircle.position;
            const std::vector<Vector2> boxVertices = GetBoxVertices(box->GetSize(), transformBox);
            std::vector<Vector2> axes = GetRectangleAxes(boxVertices);
            axes.push_back(GetCircleAxis(boxVertices, circleCenter)) ;
            const Vector2 *smallesAxis = nullptr;
            float overlap = INF;

            for(int i = 0; i < axes.size(); i++)
            {
                const Vector2 p1 = ProjectCircle(axes[i], circleCenter, circle->GetRadius() * transformCircle.scale.x);
                const Vector2 p2 = Project(boxVertices, axes[i]);

                if(!Overlap(p1, p2))
                {
                    return CollisionPoints();
                }
                const float o = GetOverlap(p1, p2);
                if(o < overlap)
                {
                    overlap = o;
                    smallesAxis = &axes[i];
                }
            }
            return CollisionPoints{*smallesAxis, overlap, true};
        }
        
        CollisionPoints FindBoxCircleCollision(
            const BoxCollider *box, Transform transformBox,
            const CircleCollider *circle, Transform transformCircle)
        {
            CollisionPoints p = FindCircleBoxCollision(circle, transformCircle, box, transformBox);
            p.Normal *= -1.0f;
            return p;
        }

        CollisionPoints FindBoxBoxCollision(
            const BoxCollider *boxA, Transform transformBoxA,
            const BoxCollider *boxB, Transform transformBoxB)
        {
            const std::vector<Vector2> verticesA = GetBoxVertices(boxA->GetSize(), transformBoxA);
            const std::vector<Vector2> verticesB = GetBoxVertices(boxB->GetSize(), transformBoxB);
            const std::vector<Vector2> axesA = GetRectangleAxes(verticesA);
            const std::vector<Vector2> axesB = GetRectangleAxes(verticesB);
            const Vector2 *smallestAxis = nullptr;
            float overlap = INF;

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

        bool Overlap(Vector2 A, Vector2 B)
        {
            if(A.y >= B.x && B.y >= A.x)
            {
                return true;
            }
            return false;
        }
        
        float GetOverlap(Vector2 A, Vector2 B)
        {
            const float overlapStart = std::max(A.x, B.x);
            const float overlapEnd = std::min(A.y, B.y);

            return overlapEnd - overlapStart;
        }

        std::vector<Vector2> GetAxes(const std::vector<Vector2> &vertices)
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

        std::vector<Vector2> GetRectangleAxes(const std::vector<Vector2> &vertices)
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
        
        Vector2 Project(const std::vector<Vector2> &vertices, Vector2 axis)
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
        
        Vector2 ProjectCircle(Vector2 axis, Vector2 circleCenter, float radius)
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
        
        void RotateVertices(std::vector<Vector2> &vertices, Vector2 center, float angle)
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
        
        std::vector<Vector2> GetBoxVertices(Vector2 boxSize, Transform transform)
        {
            Vector2 center = transform.position;
            Vector2 scale = transform.scale;
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
        
        Vector2 GetCircleAxis(const std::vector<Vector2> &vertices, Vector2 circleCenter)
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
}  