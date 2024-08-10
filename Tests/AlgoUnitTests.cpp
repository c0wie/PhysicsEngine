#include <gtest/gtest.h>
#include "../pe2d/include/Algo.hpp"

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