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
            const Vector2 centerA = transformCircleA.position;
            const Vector2 centerB = transformCircleB.position;

            const Vector2 diff = centerA - centerB;
            const float &length = Length(diff);
            // with circle I scaled them based on x value of scale
            float sum = ( radiusA * transformCircleA.scale.x ) + ( radiusB * transformCircleB.scale.x );

            if(length > sum)
            {
                return CollisionPoints();
            }

            const float overlap = sum - length;
            Vector2 normal = Normalize(diff);

            const Vector2 contactPoint = centerA - normal * radiusA;
            return CollisionPoints(normal, overlap, contactPoint, true);
        }

        CollisionPoints FindCircleBoxCollision(
            const CircleCollider *circle, Transform transformCircle,
            const BoxCollider *box, Transform transformBox)
        {
            Vector2 circleCenter = transformCircle.position;
            const std::array<Vector2, 4> boxVertices = GetBoxVertices(box->GetSize(), transformBox);
            const std::array<Vector2, 2> boxAxes = GetBoxAxes(boxVertices);
            const std::array<Vector2, 3> allAxes = { boxAxes[0], boxAxes[1], GetCircleAxis(boxVertices, circleCenter) };
            const Vector2 *smallesAxis = nullptr;
            float overlap = INF;

            for(int i = 0; i < allAxes.size(); i++)
            {
                const Vector2 p1 = ProjectCircle(circleCenter, circle->GetRadius() * transformCircle.scale.x, allAxes[i]);
                const Vector2 p2 = Project(boxVertices, allAxes[i]);

                if(!Overlap(p1, p2))
                {
                    return CollisionPoints();
                }
                const float o = GetOverlap(p1, p2);
                if(o < overlap)
                {
                    overlap = o;
                    smallesAxis = &allAxes[i];
                }
            }
            const Vector2 contactPoint = FindCircleBoxContactPoint(boxVertices, circleCenter);
            return CollisionPoints(*smallesAxis, overlap, contactPoint, true);
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
            const std::array<Vector2, 4> verticesA = GetBoxVertices(boxA->GetSize(), transformBoxA);
            const std::array<Vector2, 4> verticesB = GetBoxVertices(boxB->GetSize(), transformBoxB);
            const std::array<Vector2, 2> axesA = GetBoxAxes(verticesA);
            const std::array<Vector2, 2> axesB = GetBoxAxes(verticesB);
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
            
            std::pair<Vector2, Vector2> contactPoints = FindBoxBoxContactPoint(verticesA, verticesB);
            if(contactPoints.second == Vector2(-1.0f, -1.0f))
            {
                return CollisionPoints(*smallestAxis, overlap, contactPoints.first, true);
            }
            return CollisionPoints(*smallestAxis, overlap, contactPoints.first, contactPoints.second, true);
        }
        
        Vector2 FindCircleBoxContactPoint(const std::array<Vector2, 4> &boxVertices, Vector2 circleCenter)
        {
            float minDistanceSquared = INF;
            Vector2 contactPoint = Vector2(0.0f, 0.0f);
            for(int i = 0; i < boxVertices.size(); i++)
            {
                float distanceToCircleCenterSquared = 0.0f;
                Vector2 cp = Vector2(0.0f, 0.0f);
                const Vector2 p1 = boxVertices[i];
                const Vector2 p2 = boxVertices[(i + 1) % boxVertices.size()];
                
                PointSegmentDistance(circleCenter, p1, p2, distanceToCircleCenterSquared, cp);
                if(distanceToCircleCenterSquared < minDistanceSquared)
                {
                    minDistanceSquared = distanceToCircleCenterSquared;
                    contactPoint = cp;
                }
            }
            return contactPoint;
        }
        
        std::pair<Vector2, Vector2> FindBoxBoxContactPoint(const std::array<Vector2, 4> &boxVerticesA, const std::array<Vector2, 4> &boxVerticesB)
        {
            constexpr float error = 0.00005f;
            float minDistanceSquared = INF;
            Vector2 contactPoint1 = Vector2(0.0f, 0.0f);
            Vector2 contactPoint2 = Vector2(0.0f, 0.0f);
            int contactCount = 1;

            for(int i = 0; i < boxVerticesA.size(); i++)
            {
                for(int j = 0; j < boxVerticesB.size(); j++)
                {
                    float distanceFromVertexAToEdgeBSquared = 0.0f;
                    Vector2 cp = Vector2(0.0f, 0.0f);
                    const Vector2 p1 = boxVerticesB[j];
                    const Vector2 p2 = boxVerticesB[(j + 1) % boxVerticesB.size()];

                    PointSegmentDistance(boxVerticesA[i], p1, p2, distanceFromVertexAToEdgeBSquared, cp);
                    if(NearlyEquel(distanceFromVertexAToEdgeBSquared, minDistanceSquared, error))
                    {
                        if(!NearlyEquel(cp, contactPoint1, error))
                        {
                            contactPoint2 = cp;
                            contactCount = 2;
                        }
                    }
                    else if(distanceFromVertexAToEdgeBSquared < minDistanceSquared)
                    {
                        minDistanceSquared = distanceFromVertexAToEdgeBSquared;
                        contactPoint1 = cp;
                        contactCount = 1;
                    }
                }
            }

            for(int i = 0; i < boxVerticesB.size(); i++)
            {
                for(int j = 0; j < boxVerticesA.size(); j++)
                {
                    float distanceFromVertexBToEdgeASquared = 0.0f;
                    Vector2 cp = Vector2(0.0f, 0.0f);
                    const Vector2 p1 = boxVerticesA[j];
                    const Vector2 p2 = boxVerticesA[(j + 1) % boxVerticesA.size()];

                    PointSegmentDistance(boxVerticesB[i], p1, p2, distanceFromVertexBToEdgeASquared, cp);
                    if(NearlyEquel(distanceFromVertexBToEdgeASquared, minDistanceSquared, error))
                    {
                        if(!NearlyEquel(cp, contactPoint1, error))
                        {
                            contactPoint2 = cp;
                            contactCount = 2;
                        }
                    }
                    else if(distanceFromVertexBToEdgeASquared < minDistanceSquared)
                    {
                        minDistanceSquared = distanceFromVertexBToEdgeASquared;
                        contactPoint1 = cp;
                        contactCount = 1;
                    }
                }
            }

            if(contactCount == 2)
            {
                return std::make_pair(contactPoint1, contactPoint2);
            }
            return std::make_pair(contactPoint1, Vector2(-1.0f, -1.0f));
        }
        
        void PointSegmentDistance(Vector2 point, Vector2 vertexA, Vector2 vertexB, float &distanceSquared, Vector2 &contactPoint)
        {
            const Vector2 ab = vertexB - vertexA;
            const Vector2 ap = point - vertexA;
            const float proj = Dot(ap, ab);
            const float abLengthSquared = SquaredLength(ab);
            // relative position of the projection of the point onto the line segment
            const float d = proj / abLengthSquared;

            if(d <= 0.0f)
            {
                contactPoint = vertexA;
            }
            else if(d >= 1.0f)
            {
                contactPoint = vertexB;
            }
            else
            {
                contactPoint = vertexA + ab * d;
            }
            distanceSquared = SquaredDistance(point, contactPoint);
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
                const Vector2 normal = Normalize(Perp(edge));
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
                const Vector2 normal = Normalize(Perp(edge));
                axes[i] = normal;
            }
            return axes;
        }
        
        Vector2 ProjectCircle(Vector2 circleCenter, float radius, Vector2 axis)
        {
            const Vector2 dir = axis * radius;

            const Vector2 p1 = circleCenter + dir;
            const Vector2 p2 = circleCenter - dir;

            float min = Dot(p1, axis);
            float max = Dot(p2, axis);

            if(min > max)
            {
                std::swap(min, max);
            } 
            return Vector2{min, max};
        }
    }
}  