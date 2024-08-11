#include <gtest/gtest.h>
#include "../pe2d/include/Algo.hpp"

namespace
{
    TEST(RotateVertices, acuteAngles)
    {
        std::array<pe2d::Vector2, 4> testVertices1 = 
        {
            pe2d::Vector2(450.0f, 450.0f),
            pe2d::Vector2(550.0f, 450.0f),
            pe2d::Vector2(550.0f, 550.0f),
            pe2d::Vector2(450.0f, 550.0f)
        };
        std::array<pe2d::Vector2, 4> expectedVertices1 = 
        {
            pe2d::Vector2(499.999969f, 429.289307f), 
            pe2d::Vector2(570.710693f, 499.999969f),
            pe2d::Vector2(500.000031f, 570.710693f), 
            pe2d::Vector2(429.289307f, 500.000031f) 
        };
        const float angle1 = 45.0f * (PI / 180.0f);
        pe2d::algo::RotateVertices<std::array<pe2d::Vector2, 4>>(testVertices1, pe2d::Vector2(500.0f, 500.0f), angle1);
        EXPECT_EQ(testVertices1, expectedVertices1);

        std::vector<pe2d::Vector2> testVertices2 = 
        {
            pe2d::Vector2(450.0f, 450.0f),
            pe2d::Vector2(550.0f, 450.0f),
            pe2d::Vector2(550.0f, 550.0f),
            pe2d::Vector2(450.0f, 550.0f)
        };
        std::vector<pe2d::Vector2> expectedVertices2 = 
        {
            pe2d::Vector2(469.002472f, 436.445679f),
            pe2d::Vector2(563.554321f, 469.002472f),
            pe2d::Vector2(530.997559f, 563.554321f),
            pe2d::Vector2(436.445679f, 530.997559f)
        };
        const float angle2 = 19.0f * (PI / 180.0f);
        pe2d::algo::RotateVertices<std::vector<pe2d::Vector2>>(testVertices2, pe2d::Vector2(500.0f, 500.0f), angle2);
        EXPECT_EQ(testVertices2, expectedVertices2);
    }

    // might consider resolving this rotation in diffrent way
    TEST(RotateVertices, rightAngles)
    {
        const float angle = 90.0f * (PI / 180.0f);
        std::array<pe2d::Vector2, 4> testVertices1 = 
        {
            pe2d::Vector2(375.0f, 375.0f),
            pe2d::Vector2(525.0f, 375.0f),
            pe2d::Vector2(525.0f, 525.0f),
            pe2d::Vector2(375.0f, 525.0f)
        };
        std::array<pe2d::Vector2, 4> expectedVertices1 =
        {
            pe2d::Vector2(524.999939f, 374.999908f),
            pe2d::Vector2(525.000061f, 524.999939f),
            pe2d::Vector2(375.000092f, 525.000061f),
            pe2d::Vector2(374.999908f, 375.000092f)
        };
        pe2d::algo::RotateVertices<std::array<pe2d::Vector2, 4>>(testVertices1, pe2d::Vector2(450.0f, 450.0f), angle);
        EXPECT_EQ(testVertices1, expectedVertices1);

        std::vector<pe2d::Vector2> testVertices2 = 
        {
            pe2d::Vector2(500.0f, 450.0f),
            pe2d::Vector2(700.0f, 450.0f),
            pe2d::Vector2(700.0f, 750.0f),
            pe2d::Vector2(500.0f, 750.0f) 
        };
        std::vector<pe2d::Vector2> expectedVertices2 =
        {
            pe2d::Vector2(749.999878f, 499.999817f),
            pe2d::Vector2(750.000122f, 699.999817f),
            pe2d::Vector2(450.000122f, 700.000183f),
            pe2d::Vector2(449.999878f, 500.000183f) 
        };
        pe2d::algo::RotateVertices<std::vector<pe2d::Vector2>>(testVertices2, pe2d::Vector2(600.0f, 600.0f), angle);
        EXPECT_EQ(testVertices2, expectedVertices2);
    }

    TEST(RotateVertices, obtuseAngles)
    {
        const float angle1 = 189.0f * (PI / 180.0f);
        std::array<pe2d::Vector2, 4> testVertices1 = 
        {
            pe2d::Vector2(339.5f, 170.f),
            pe2d::Vector2(418.5f, 170.f),
            pe2d::Vector2(418.5f, 252.f),
            pe2d::Vector2(339.5f, 252.f)
        };
        const std::array<pe2d::Vector2, 4> expectedVertices1 = 
        {
            pe2d::Vector2(411.600006f, 257.674316f),
            pe2d::Vector2(333.572571f, 245.316177f),
            pe2d::Vector2(346.399994f, 164.325699f),
            pe2d::Vector2(424.427429f, 176.683823f)
        };
        pe2d::algo::RotateVertices<std::array<pe2d::Vector2, 4>>(testVertices1, pe2d::Vector2(379.0f, 211.0f), angle1);
        const float angle2 = 219.0f * (PI / 180.0f);
        std::vector<pe2d::Vector2> testVertices2 = 
        {
            pe2d::Vector2(465.5f, 300.f),
            pe2d::Vector2(534.5f, 300.f),
            pe2d::Vector2(534.5f, 700.f),
            pe2d::Vector2(465.5f, 700.f)
        };
        const std::vector<pe2d::Vector2> expectedVertices2 = 
        {
            pe2d::Vector2(400.947968f, 677.140991f),
            pe2d::Vector2(347.324768f, 633.718079f),
            pe2d::Vector2(599.052002f, 322.859009f),
            pe2d::Vector2(652.675232f, 366.281921f)
        };
        pe2d::algo::RotateVertices<std::vector<pe2d::Vector2>>(testVertices2, pe2d::Vector2(500.0f, 500.0f), angle2);
        EXPECT_EQ(testVertices2, expectedVertices2);
    }
    
    TEST(GetVerticesTest, notRotatednotScaled)
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

    TEST(GetVerticesTest, rotatedScaled)
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
    }

    TEST(OverlapTest, notOverlaping)
    {
        const pe2d::Vector2 proj1 = pe2d::Vector2(1.0f, 3.0f);
        const pe2d::Vector2 proj2 = pe2d::Vector2(3.1f, 7.0f);
        EXPECT_EQ(pe2d::algo::Overlap(proj1, proj2), false);
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
}