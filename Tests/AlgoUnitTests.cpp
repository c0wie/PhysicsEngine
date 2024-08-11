#include <gtest/gtest.h>
#include "../pe2d/include/Algo.hpp"

namespace
{
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
            angle = angle * (PI / 180.0f);
            Container rotatedVerties = m_TestVertices;
            pe2d::algo::RotateVertices<Container>(rotatedVerties, m_Center, angle);
            ASSERT_EQ(rotatedVerties.size(), expectedVertices.size());
            EXPECT_EQ(rotatedVerties, expectedVertices);
        }
    protected:
        Container m_TestVertices;
        pe2d::Vector2 m_Center;
    };
    using testing::Types;
    typedef Types<std::array<pe2d::Vector2, 4>, std::vector<pe2d::Vector2>> Implementations;

    TYPED_TEST_SUITE(RotateVerticesTest, Implementations);

    TYPED_TEST(RotateVerticesTest, acuteAngles)
    {
        const TypeParam expectedVertices1 = 
        {
            pe2d::Vector2(499.999969f, 429.289307f), 
            pe2d::Vector2(570.710693f, 499.999969f),
            pe2d::Vector2(500.000031f, 570.710693f), 
            pe2d::Vector2(429.289307f, 500.000031f) 
        };
        this->CheckRotation(45.0f, expectedVertices1);

        const TypeParam expectedVertices2
        {
            pe2d::Vector2(464.644653f, 438.762756f),
            pe2d::Vector2(561.237244f, 464.644653f),
            pe2d::Vector2(535.355347f, 561.237244f),
            pe2d::Vector2(438.762756f, 535.355347f)
        };
        this->CheckRotation(15.0f, expectedVertices2);
    }

    TYPED_TEST(RotateVerticesTest, rightAngle)
    {
        const TypeParam expectedVertices =
        {
            pe2d::Vector2(549.999939f, 449.999939f),
            pe2d::Vector2(550.000061f, 549.999939f),
            pe2d::Vector2(450.000061f, 550.000061f),
            pe2d::Vector2(449.999939f, 450.000061f)
        };
        this->CheckRotation(90.0f, expectedVertices);
    }

    TYPED_TEST(RotateVerticesTest, obtuseAngles)
    {
        const TypeParam expectedVertices1 = 
        {
            pe2d::Vector2(541.562866f, 557.206055f),
            pe2d::Vector2(442.793945f, 541.562866f),
            pe2d::Vector2(458.437164f, 442.793945f),
            pe2d::Vector2(557.206055f, 458.437164f)
        };
        this->CheckRotation(189.0f, expectedVertices1);
        const TypeParam expectedVertices2 = 
        {
            pe2d::Vector2(516.507263f, 568.756897f),
            pe2d::Vector2(431.243103f, 516.507263f),
            pe2d::Vector2(483.492737f, 431.243103f),
            pe2d::Vector2(568.756897f, 483.492737f)
        };
        this->CheckRotation(211.5f, expectedVertices2);
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
        const pe2d::Transform boxTransform = pe2d::Transform(pe2d::Vector2(500.0f, 500.0f), pe2d::Vector2(1.0f, 2.0f), 45.0f);
        std::array<pe2d::Vector2, 4> expected = 
        {
            pe2d::Vector2(535.355286f, 393.93396f),
            pe2d::Vector2(606.065979f, 464.644592f),
            pe2d::Vector2(464.644714f, 606.06604f),
            pe2d::Vector2(393.934021f, 535.355408f)
        };
        EXPECT_EQ(pe2d::algo::GetBoxVertices(boxSize, boxTransform), expected);
    }

    TEST(OverlapTest, isOverlaping)
    {
        const pe2d::Vector2 proj1 = pe2d::Vector2(1.0f, 3.0f);
        const pe2d::Vector2 proj2 = pe2d::Vector2(3.0f, 7.0f);
        EXPECT_EQ(pe2d::algo::Overlap(proj1, proj2), true);

        const pe2d::Vector2 proj3 = pe2d::Vector2(1.12345f, 3.2137f);
        const pe2d::Vector2 proj4 = pe2d::Vector2(3.2137f, 7.42f);
        EXPECT_EQ(pe2d::algo::Overlap(proj3, proj4), true);
    }

    TEST(OverlapTest, notOverlaping)
    {
        const pe2d::Vector2 proj1 = pe2d::Vector2(1.0f, 3.0f);
        const pe2d::Vector2 proj2 = pe2d::Vector2(3.1f, 7.0f);
        EXPECT_EQ(pe2d::algo::Overlap(proj1, proj2), false);

        const pe2d::Vector2 proj3 = pe2d::Vector2(1100.0f, 3000.0f);
        const pe2d::Vector2 proj4 = pe2d::Vector2(-700.1f, -300.0f);
        EXPECT_EQ(pe2d::algo::Overlap(proj3, proj4), false);
    }

    TEST(GetRectangleAxesTest, notRotatednotScaled)
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
        EXPECT_EQ(pe2d::algo::GetRectangleAxes(vertices), axes);
    }

    TEST(Project, alignedAxis)
    {
        const std::array<pe2d::Vector2, 4> testVertices1 = 
        {
            pe2d::Vector2(375.0f, 375.0f),
            pe2d::Vector2(525.0f, 375.0f),
            pe2d::Vector2(525.0f, 525.0f),
            pe2d::Vector2(375.0f, 525.0f)
        };
        const pe2d::Vector2 testAxis1 = pe2d::Vector2(0.0f, 1.0f);
        const pe2d::Vector2 expectedProjection1 = pe2d::Vector2(375.0f, 525.0f);
        pe2d::Vector2 projection1 = pe2d::algo::Project(testVertices1, testAxis1);
        EXPECT_EQ(projection1, expectedProjection1);

        const std::array<pe2d::Vector2, 4> testVertices2 = 
        {
            pe2d::Vector2(339.5f, 170.f),
            pe2d::Vector2(418.5f, 170.f),
            pe2d::Vector2(418.5f, 252.f),
            pe2d::Vector2(339.5f, 252.f)
        };
        const pe2d::Vector2 testAxis2 = pe2d::Vector2(1.0f, 0.0f);
        const pe2d::Vector2 expectedProjection2 = pe2d::Vector2(339.5f, 418.5f);
        pe2d::Vector2 projection2 = pe2d::algo::Project(testVertices2, testAxis2);
        EXPECT_EQ(projection2, expectedProjection2);
    }

    TEST(Project, nonAlignedAxis)
    {
        const std::array<pe2d::Vector2, 4> testVertices1 = 
        {
            pe2d::Vector2(535.355286f, 393.93396f),
            pe2d::Vector2(606.065979f, 464.644592f),
            pe2d::Vector2(464.644714f, 606.06604f),
            pe2d::Vector2(393.934021f, 535.355408f)
        };
        const pe2d::Vector2 testAxis1 = pe2d::Vector2(0.5f, 0.5f);
        const pe2d::Vector2 expectedProjection1 = pe2d::Vector2(464.644623f, 535.355347f);
        pe2d::Vector2 projection1 = pe2d::algo::Project(testVertices1, testAxis1);
        EXPECT_EQ(projection1, expectedProjection1);

        const std::array<pe2d::Vector2, 4> testVertices2 = 
        {
            pe2d::Vector2(499.999969f, 429.289307f), 
            pe2d::Vector2(570.710693f, 499.999969f),
            pe2d::Vector2(500.000031f, 570.710693f), 
            pe2d::Vector2(429.289307f, 500.000031f) 
        };
        const pe2d::Vector2 testAxis2 = pe2d::Vector2(0.69f, 0.31f);
        const pe2d::Vector2 expectedProjection2 = pe2d::Vector2(451.209656f, 548.790344f);
        pe2d::Vector2 projection2 = pe2d::algo::Project(testVertices2, testAxis2);
        EXPECT_EQ(projection2, expectedProjection2);
    }

    TEST(ProjectCircle, alignedAxis)
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

    TEST(ProjectCircle, nonAlignedAxis)
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
}