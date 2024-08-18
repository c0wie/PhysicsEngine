#include <cmath>
#include "Algo.hpp"
#include "Collider.hpp"

namespace pe2d
{
    namespace algo
    {
        bool FindCircleCircleCollision(const CircleCollider *circleA, Transform transformCircleA, const CircleCollider *circleB, Transform transformCircleB, Vector2 &normal, float &depth)
        {
            const float radiusA = circleA->GetRadius();
            const float radiusB =  circleB->GetRadius();
            const Vector2 centerA = transformCircleA.position;
            const Vector2 centerB = transformCircleB.position;

            const Vector2 diff = centerA - centerB;
            const float &length = diff.magnitude();
            float sum = ( radiusA * transformCircleA.scale.x ) + ( radiusB * transformCircleB.scale.x );

            if(length > sum)
            {
                depth = 0.0f;
                normal = Vector2(0.0f, 0.0f);
                return false;
            }

            depth = sum - length;
            normal = diff.normalized();

            return true;
        }

        bool FindCircleBoxCollision(const CircleCollider *circle, Transform transformCircle, const BoxCollider *box, Transform transformBox, Vector2 &normal, float &depth)
        {
            Vector2 circleCenter = transformCircle.position;
            const std::array<Vector2, 4> boxVertices = GetBoxVertices(box->GetSize(), transformBox);
            const std::array<Vector2, 2> axes = GetBoxAxes(boxVertices);
            depth = INF;

            for(int i = 0; i < axes.size(); i++)
            {
                const Vector2 p1 = ProjectCircle(circleCenter, circle->GetRadius() * transformCircle.scale.x, axes[i]);
                const Vector2 p2 = Project(boxVertices, axes[i]);

                if(!Overlap(p1, p2))
                {  
                    depth = 0.0f;
                    normal = Vector2(0.0f, 0.0f);
                    return false;
                }
                const float o = GetOverlap(p1, p2);
                if(o < depth)
                {
                    depth = o;
                    normal = axes[i];
                }
            }
            const Vector2 circleAxis = GetCircleAxis(boxVertices, circleCenter); 
            const Vector2 p1 = ProjectCircle(circleCenter, circle->GetRadius() * transformCircle.scale.x, circleAxis);
            const Vector2 p2 = Project(boxVertices, circleAxis);

            if(!Overlap(p1, p2))
            {
                depth = 0.0f;
                normal = Vector2(0.0f, 0.0f);
                return false;
            }
            const float o = GetOverlap(p1, p2);
            if(o < depth)
            {
                depth = o;
                normal = circleAxis;
            }
            return true;
        }
        
        bool FindBoxCircleCollision(const BoxCollider *box, Transform transformBox, const CircleCollider *circle, Transform transformCircle, Vector2 &normal, float &depth)
        {
            bool p = FindCircleBoxCollision(circle, transformCircle, box, transformBox, normal, depth);
            normal *= -1.0f;
            return p;
        }

        bool FindBoxBoxCollision(const BoxCollider *boxA, Transform transformBoxA, const BoxCollider *boxB, Transform transformBoxB, Vector2 &normal, float &depth)
        {
            const std::array<Vector2, 4> verticesA = GetBoxVertices(boxA->GetSize(), transformBoxA);
            const std::array<Vector2, 4> verticesB = GetBoxVertices(boxB->GetSize(), transformBoxB);
            const std::array<Vector2, 2> axesA = GetBoxAxes(verticesA);
            const std::array<Vector2, 2> axesB = GetBoxAxes(verticesB);
            depth = INF;

            for(int i = 0; i < axesA.size(); i++)
            {
                const Vector2 pA1 = Project(verticesA, axesA[i]);
                const Vector2 pA2 = Project(verticesB, axesA[i]);  

                if(!Overlap(pA1, pA2))
                {
                    depth = 0.0f;
                    normal = Vector2(0.0f, 0.0f);
                    return false;
                }

                const Vector2 pB1 = Project(verticesA, axesB[i]);
                const Vector2 pB2 = Project(verticesB, axesB[i]);

                if(!Overlap(pB1, pB2))
                {
                    depth = 0.0f;
                    normal = Vector2(0.0f, 0.0f);
                    return false;
                }

                const float overlapA = GetOverlap(pA1, pA2);
                const float overlapB = GetOverlap(pB1, pB2);

                if(overlapA < depth)
                {
                    depth = overlapA;
                    normal = axesA[i];
                }
                if(overlapB < depth)
                {
                    depth = overlapB;
                    normal = axesB[i];
                }
            }
            return true;
        }

        std::array<Vector2, 4> GetBoxVertices(Vector2 boxSize, Transform transform)
        {
            Vector2 center = transform.position;
            Vector2 scale = transform.scale;
            const float scaledHalfSizeX = (boxSize.x * scale.x) / 2.0f;
            const float scaledHalfSizeY = (boxSize.y * scale.y) / 2.0f;
            const float angle = transform.GetRotationInRadians();
            std::array<Vector2, 4> vertices = 
            {
                Vector2( center.x - scaledHalfSizeX, center.y - scaledHalfSizeY ),
                Vector2( center.x + scaledHalfSizeX, center.y - scaledHalfSizeY ),
                Vector2( center.x + scaledHalfSizeX, center.y + scaledHalfSizeY ),
                Vector2( center.x - scaledHalfSizeX, center.y + scaledHalfSizeY )
            };
            RotateVertices<std::array<Vector2, 4>>(vertices, center, angle);
            return vertices;
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

        std::array<Vector2, 2> GetBoxAxes(const std::array<Vector2, 4> &vertices)
        {
            std::array<Vector2, 2> axes;
            // is has two parrarel edges so I don't have to check other two
            for(int i = 0; i < 2; i++)
            {
                const Vector2 p1 = vertices[i];
                const Vector2 p2 = vertices[(i + 1) % vertices.size()];
                const Vector2 edge = p1 - p2;
                const Vector2 normal = edge.perp().normalized();
                axes[i] = normal;
            }
            return axes;
        }
        
        Vector2 ProjectCircle(Vector2 circleCenter, float radius, Vector2 axis)
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
    }
}  