#include <gtest/gtest.h>
#include "Algo.hpp"
#include "Math.hpp"

namespace
{
    using ::testing::Types;

    TEST(FindCircleBoxContactPointTest, normalScenario)
    {
        const std::array<pe2d::Vector2, 4> boxVertices = 
        {
            pe2d::Vector2(450.0f, 450.0f),
            pe2d::Vector2(550.0f, 450.0f),
            pe2d::Vector2(550.0f, 550.0f),
            pe2d::Vector2(450.0f, 550.0f)
        };
        const pe2d::Vector2 circleCenter = pe2d::Vector2(575.0f, 575.0f);
        const pe2d::Vector2 contactPoint = pe2d::algo::FindCircleBoxContactPoint(boxVertices, circleCenter);
        const pe2d::Vector2 expectedContactPoint = pe2d::Vector2(550.0f, 550.0f);
        EXPECT_EQ(contactPoint.x, expectedContactPoint.x);
        EXPECT_EQ(contactPoint.y, expectedContactPoint.y);
    }

    class FindBoxBoxContactPointTest : public testing::Test
    {
    protected:
        void SetUp() override
        {
            boxVerices = 
            {
                pe2d::Vector2(100.0f, 750.0f),
                pe2d::Vector2(900.0f, 750.0f),
                pe2d::Vector2(900.0f, 850.0f),
                pe2d::Vector2(100.0f, 850.0f)
            };
        }
        void CheckContactPoints(std::array<pe2d::Vector2, 4> vertices, std::pair<pe2d::Vector2, pe2d::Vector2> expectedPoints)
        {
            std::array<pe2d::Vector2, 4> ver = boxVerices;
            std::pair<Vector2, Vector2> points = pe2d::algo::FindBoxBoxContactPoint(vertices, ver);
            EXPECT_EQ(points.first.x, expectedPoints.first.x);
            EXPECT_EQ(points.first.y, expectedPoints.first.y);
            EXPECT_EQ(points.second.x, expectedPoints.second.x);
            EXPECT_EQ(points.second.y, expectedPoints.second.y);
        }
    protected:
        std::array<pe2d::Vector2, 4> boxVerices;
    };

    TEST_F(FindBoxBoxContactPointTest, oneContactPoint)
    {
        std::array<pe2d::Vector2, 4> vertices = 
        {   
            pe2d::Vector2(733.425659f, 624.799377f),
            pe2d::Vector2(733.921265f, 730.293945f),
            pe2d::Vector2(722.574341f, 750.004211f),
            pe2d::Vector2(682.078735f, 682.078735f)
        };
        std::pair<pe2d::Vector2, pe2d::Vector2> expectedPoints = std::make_pair(pe2d::Vector2(722.574341f, 750), pe2d::Vector2(-1.0f, -1.0f));
        this->CheckContactPoints(vertices, expectedPoints);
    }

    TEST_F(FindBoxBoxContactPointTest, twoContactPoints)
    {
        std::array<pe2d::Vector2, 4> vertices = 
        {   
            pe2d::Vector2(487.5f, 673.0f),
            pe2d::Vector2(634.5, 673.0f),
            pe2d::Vector2(634.5, 750.0f),
            pe2d::Vector2(487.5, 750.0f)
        };
        std::pair<pe2d::Vector2, pe2d::Vector2> expectedPoints = std::make_pair(pe2d::Vector2(634.5, 750.0f), pe2d::Vector2(487.5f, 750.0f));
        this->CheckContactPoints(vertices, expectedPoints);

    }

    TEST(PointSegmentDistanceTest, pointOutsideEdge)
    {
        const pe2d::Vector2 point = pe2d::Vector2(5.0f, 5.0f);
        const pe2d::Vector2 vertexA = pe2d::Vector2(0.0f, 0.0f);
        const pe2d::Vector2 vertexB = pe2d::Vector2(10.0f, 0.0f);
        float distanceSquared = 0.0f;
        pe2d::Vector2 contactPoint = pe2d::Vector2(0.0f, 0.0f);
        const pe2d::Vector2 expectedContactPoint = pe2d::Vector2(5.0f, 0.0f);
        const float expectedDistance = 25.0f;
        pe2d::algo::PointSegmentDistance(point, vertexA, vertexB, distanceSquared, contactPoint);
        EXPECT_EQ(distanceSquared, expectedDistance);
        EXPECT_EQ(contactPoint.x, expectedContactPoint.x);
        EXPECT_EQ(contactPoint.y, expectedContactPoint.y);
    }

    TEST(PointSegmentDistanceTest, pointOnEdge)
    {
        const pe2d::Vector2 point = pe2d::Vector2(5.0f, 5.0f);
        const pe2d::Vector2 vertexA = pe2d::Vector2(0.0f, 5.0f);
        const pe2d::Vector2 vertexB = pe2d::Vector2(10.0f, 5.0f);
        float distanceSquared = 0.0f;
        pe2d::Vector2 contactPoint = pe2d::Vector2(0.0f, 0.0f);
        const pe2d::Vector2 expectedContactPoint = pe2d::Vector2(5.0f, 5.0f);
        const float expectedDistance = 0.0f;
        pe2d::algo::PointSegmentDistance(point, vertexA, vertexB, distanceSquared, contactPoint);
        EXPECT_EQ(distanceSquared, expectedDistance);
        EXPECT_EQ(contactPoint.x, expectedContactPoint.x);
        EXPECT_EQ(contactPoint.y, expectedContactPoint.y);
    }
    template <typename Container>
    class RotateVerticesTest : public testing::Test
    {
    protected:
        void SetUp() override
        {
            m_Center = pe2d::Vector2(500.0f, 500.0f);
            m_TestVertices = 
            {
                pe2d::Vector2(450.0f, 450.0f),
                pe2d::Vector2(550.0f, 450.0f),
                pe2d::Vector2(550.0f, 550.0f),
                pe2d::Vector2(450.0f, 550.0f)
            };
        }
        // angle in deegres
        void CheckRotation(float angle, const Container &expectedVertices)
        {
            angle = angle * (pe2dMath::PI / 180.0f);
            Container rotatedVerties = m_TestVertices;
            pe2d::algo::RotateVertices<Container>(rotatedVerties, m_Center, angle);
            ASSERT_EQ(rotatedVerties.size(), expectedVertices.size());
            for(int i = 0; i < expectedVertices.size(); i++)
            {
                EXPECT_EQ(rotatedVerties[i].x, expectedVertices[i].x);
                EXPECT_EQ(rotatedVerties[i].y, expectedVertices[i].y);
            }
        }
    protected:
        Container m_TestVertices;
        pe2d::Vector2 m_Center;
    };
    typedef Types<std::array<pe2d::Vector2, 4>, std::vector<pe2d::Vector2>> Implementations;

    TYPED_TEST_SUITE(RotateVerticesTest, Implementations);

    TYPED_TEST(RotateVerticesTest, acuteAngles)
    {
        const TypeParam expectedVertices1 = 
        {
            pe2d::Vector2(500.0f, 429.289307f), 
            pe2d::Vector2(570.710693f, 500.0f),
            pe2d::Vector2(500.0f, 570.710693f), 
            pe2d::Vector2(429.289307f, 500.0f) 
        };
        this->CheckRotation(pe2dMath::DeegresToRadians(45.0f), expectedVertices1);

        const TypeParam expectedVertices2
        {
            pe2d::Vector2(464.644653f, 438.762756f),
            pe2d::Vector2(561.237244f, 464.644653f),
            pe2d::Vector2(535.355347f, 561.237244f),
            pe2d::Vector2(438.762756f, 535.355347f)
        };
        this->CheckRotation(pe2dMath::DeegresToRadians(15.0f), expectedVertices2);
    }

    TYPED_TEST(RotateVerticesTest, rightAngle)
    {
        const TypeParam expectedVertices =
        {
            pe2d::Vector2(550.0f, 450.0f),
            pe2d::Vector2(550.0f, 550.0f),
            pe2d::Vector2(450.0f, 550.0f),
            pe2d::Vector2(450.0f, 450.0f)
        };
        this->CheckRotation(pe2dMath::DeegresToRadians(90.0f), expectedVertices);
    }

    TYPED_TEST(RotateVerticesTest, obtuseAngles)
    {
        const TypeParam expectedVertices1 = 
        {
            pe2d::Vector2(541.562683f, 557.206116f),
            pe2d::Vector2(442.793854f, 541.562683f),
            pe2d::Vector2(458.437317f, 442.793854f),
            pe2d::Vector2(557.206116f, 458.437317f)
        };
        this->CheckRotation(pe2dMath::DeegresToRadians(189.0f), expectedVertices1);
        const TypeParam expectedVertices2 = 
        {
            pe2d::Vector2(516.50708f, 568.756958f),
            pe2d::Vector2(431.243042f, 516.50708f),
            pe2d::Vector2(483.49292f, 431.243042f),
            pe2d::Vector2(568.756958f, 483.49292f)
        };
        this->CheckRotation(pe2dMath::DeegresToRadians(211.5f), expectedVertices2);
    }
    
    TEST(GetBoxVerticesTest, notRotatednotScaled)
    {
        const pe2d::Vector2 boxSize = pe2d::Vector2(100.0f, 100.0f);
        const pe2d::Transform boxTransform = pe2d::Transform(pe2d::Vector2(500.0f, 500.0f), pe2d::Vector2(1.0f, 1.0f), 0.0f);
        std::array<pe2d::Vector2, 4> expected = 
        {
            pe2d::Vector2(450.0f, 450.0f),
            pe2d::Vector2(550.0f, 450.0f),
            pe2d::Vector2(550.0f, 550.0f),
            pe2d::Vector2(450.0f, 550.0f)
        };
        EXPECT_EQ(pe2d::algo::GetBoxVertices(boxSize, boxTransform), expected);
    }

    TEST(GetBoxVerticesTest, rotatedScaled)
    {
        const pe2d::Vector2 boxSize = pe2d::Vector2(100.0f, 100.0f);
        const pe2d::Transform boxTransform = pe2d::Transform(pe2d::Vector2(500.0f, 500.0f), pe2d::Vector2(1.0f, 2.0f),  pe2dMath::DeegresToRadians(45.0f));
        std::array<pe2d::Vector2, 4> expected = 
        {
            pe2d::Vector2(535.355347f, 393.93399f),
            pe2d::Vector2(606.06604f, 464.644653f),
            pe2d::Vector2(464.644653f, 606.06604f),
            pe2d::Vector2(393.93399f, 535.355347f)
        };
        EXPECT_EQ(pe2d::algo::GetBoxVertices(boxSize, boxTransform), expected);
    }

    TEST(OverlapTest, isOverlaping)
    {
        constexpr float error = 0.0001;
        const pe2d::Vector2 proj1 = pe2d::Vector2(1.0f, 3.0f);
        const pe2d::Vector2 proj2 = pe2d::Vector2(3.0f, 7.0f);
        float overlap1 = 0.0f;
        EXPECT_EQ(pe2d::algo::Overlap(proj1, proj2, overlap1), true);
        EXPECT_NEAR(overlap1, 0.0f, error);

        const pe2d::Vector2 proj3 = pe2d::Vector2(1.12345f, 3.2138f);
        const pe2d::Vector2 proj4 = pe2d::Vector2(3.2137f, 7.42f);
        float overlap2 = 0.0f;
        EXPECT_EQ(pe2d::algo::Overlap(proj3, proj4, overlap2), true);
        EXPECT_NEAR(overlap2, 0.0001f, error);
    }

    TEST(OverlapTest, notOverlaping)
    {
        const pe2d::Vector2 proj1 = pe2d::Vector2(1.0f, 3.0f);
        const pe2d::Vector2 proj2 = pe2d::Vector2(3.1f, 7.0f);
        float overlap1 = 0.0f;
        EXPECT_EQ(pe2d::algo::Overlap(proj1, proj2, overlap1), false);
        EXPECT_EQ(overlap1, -1.0f);

        const pe2d::Vector2 proj3 = pe2d::Vector2(1100.0f, 3000.0f);
        const pe2d::Vector2 proj4 = pe2d::Vector2(-700.1f, -300.0f);
        float overlap2 = 0.0f;
        EXPECT_EQ(pe2d::algo::Overlap(proj3, proj4, overlap2), false);
        EXPECT_EQ(overlap2, -1.0f);
    }

    TEST(GetBoxAxesTest, notRotatednotScaled)
    {
        const std::array<pe2d::Vector2, 4> vertices = 
        {
            pe2d::Vector2(450.0f, 450.0f),
            pe2d::Vector2(550.0f, 450.0f),
            pe2d::Vector2(550.0f, 550.0f),
            pe2d::Vector2(450.0f, 550.0f)
        };
        const std::array<pe2d::Vector2, 2> axes =
        {
            pe2d::Vector2(0.0f, -1.0f),
            pe2d::Vector2(1.0f, 0.0f)
        };
        EXPECT_EQ(pe2d::algo::GetBoxAxes(vertices), axes);
    }

    
    template<typename Container>
    class GetCircleAxisTest : public testing::Test
    {
    protected:
        void SetUp() override
        {
            m_TestVertices = 
            {
                pe2d::Vector2(535.355286f, 393.93396f),
                pe2d::Vector2(606.065979f, 464.644592f),
                pe2d::Vector2(464.644714f, 606.06604f),
                pe2d::Vector2(393.934021f, 535.355408f)
            };
        }
        void checkAxis(pe2d::Vector2 circleCenter, pe2d::Vector2 expectedAxis)
        {
            const pe2d::Vector2 resultAxis = pe2d::algo::GetCircleAxis(this->m_TestVertices, circleCenter);
            const float error = 0.000001f;
            EXPECT_NEAR(resultAxis.x, expectedAxis.x, error);
            EXPECT_NEAR(resultAxis.y, expectedAxis.y, error);
        }
    protected:
        Container m_TestVertices;
    };

    TYPED_TEST_SUITE(GetCircleAxisTest, Implementations);

    TYPED_TEST(GetCircleAxisTest, standardScenario)
    {
        const pe2d::Vector2 circleCenter1 = pe2d::Vector2(600.0f, 300.0f);
        const pe2d::Vector2 expectedAxis1 = pe2d::Vector2(-0.566917f, 0.823775f);
        this->checkAxis(circleCenter1, expectedAxis1);

        const pe2d::Vector2 circleCenter2 = pe2d::Vector2(0.0f, 420.0f);
        const pe2d::Vector2 expectedAxis2 = pe2d::Vector2(0.959700f, 0.281028f);
        this->checkAxis(circleCenter2, expectedAxis2);
    }

}

    template<typename Container>
    class ProjectTest : public testing::Test
    {
    protected:
        void SetUp() override
        {
            m_TestVertices =
            {
                pe2d::Vector2(339.5f, 170.0f),
                pe2d::Vector2(418.5f, 170.0f),
                pe2d::Vector2(418.5f, 252.0f),
                pe2d::Vector2(339.5f, 252.0f)
            };
        }
        void CheckProjection(pe2d::Vector2 expectedProjection, pe2d::Vector2 axis)
        {
            const pe2d::Vector2 projection = pe2d::algo::Project(m_TestVertices, axis);
            EXPECT_EQ(projection.x, expectedProjection.x);
            EXPECT_EQ(projection.y, expectedProjection.y);
        }
    protected:
        Container m_TestVertices;
    };

    TYPED_TEST_SUITE(ProjectTest, Implementations);

    TYPED_TEST(ProjectTest, alignedAxis)
    {
        const pe2d::Vector2 testAxis1 = pe2d::Vector2(1.0f, 0.0f);
        const pe2d::Vector2 expectedProjection1 = pe2d::Vector2(339.5f, 418.5f);
        this->CheckProjection(expectedProjection1, testAxis1);

        const pe2d::Vector2 testAxis2 = pe2d::Vector2(0.0f, 1.0f);
        const pe2d::Vector2 expectedProjection2 = pe2d::Vector2(170.0f, 252.0f);
        this->CheckProjection(expectedProjection2, testAxis2);
    }

    TYPED_TEST(ProjectTest, nonAlignedAxis)
    {
        const pe2d::Vector2 testAxis1 = pe2d::Vector2(0.05, 0.95);
        const pe2d::Vector2 expectedProjection1 = pe2d::Vector2(178.475006f, 260.324982f);
        this->CheckProjection(expectedProjection1, testAxis1);

        const pe2d::Vector2 testAxis2 = pe2d::Vector2(0.21, 0.79);
        const pe2d::Vector2 expectedProjection2 = pe2d::Vector2(205.595001f, 286.964996f);
        this->CheckProjection(expectedProjection2, testAxis2); 
    }

    TEST(ProjectCircleTest, alignedAxis)
    {
        const pe2d::Vector2 circleCenter = pe2d::Vector2(70.0f, 800.0f);
        const float radius = 60.0f;
        const pe2d::Vector2 axis1 = pe2d::Vector2(0.0f, 1.0f);
        const pe2d::Vector2 expectedProjection1 = pe2d::Vector2(740.0f, 860.0f);
        const pe2d::Vector2 projection1 = pe2d::algo::ProjectCircle(circleCenter, radius, axis1);
        EXPECT_EQ(projection1, expectedProjection1);

        const pe2d::Vector2 axis2 = pe2d::Vector2(1.0f, 0.0f);
        const pe2d::Vector2 expectedProjection2 = pe2d::Vector2(10.0f , 130.0f);
        const pe2d::Vector2 projection2 = pe2d::algo::ProjectCircle(circleCenter, radius, axis2);
        EXPECT_EQ(projection2, expectedProjection2);
    }

    TEST(ProjectCircleTest, nonAlignedAxis)
    {
        const pe2d::Vector2 circleCenter = pe2d::Vector2(500.0f, 480.0f);
        const float radius = 78.0f;
        const pe2d::Vector2 axis1 = pe2d::Vector2(0.5f, 0.5f);
        const pe2d::Vector2 expectedProjection1 = pe2d::Vector2(451.0f, 529.0f);
        const pe2d::Vector2 projection1 = pe2d::algo::ProjectCircle(circleCenter, radius, axis1);
        EXPECT_EQ(projection1, expectedProjection1);

        const pe2d::Vector2 axis2 = pe2d::Vector2(0.7f, 0.3f);
        const pe2d::Vector2 expectedProjection2 = pe2d::Vector2(448.76001f, 539.23999f);
        const pe2d::Vector2 projection2 = pe2d::algo::ProjectCircle(circleCenter, radius, axis2);
        EXPECT_EQ(projection2, expectedProjection2);
    }