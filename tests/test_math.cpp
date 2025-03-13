// local
#include "math.hpp"
#include "vector2.hpp"

// lib
// gtest
#include <gtest/gtest.h>

namespace {
TEST(MathTest, Length) {
  const pe2d::Vec2d v1(-4.0, 4.0);
  const double v1_length = 5.65685424949238;
  EXPECT_DOUBLE_EQ(pe2d::math::Length(v1), v1_length);
}

TEST(MathTest, SquaredLength) {
  const pe2d::Vec2d v1(-4.0, 4.0);
  const double v1_length_squared = 32;
  EXPECT_DOUBLE_EQ(pe2d::math::SquaredLength(v1), v1_length_squared);
}

TEST(MathTest, Distance) {
  const pe2d::Pos2d p1(1.0, 1.0);
  const pe2d::Pos2d p2(3.0, 4.0);
  const double distance = 3.605551275463989;
  EXPECT_DOUBLE_EQ(pe2d::math::Distance(p1, p2), distance);
}

TEST(MathTest, DistanceSquared) {
  const pe2d::Pos2d p1(1.0, 1.0);
  const pe2d::Pos2d p2(3.0, 4.0);
  const double distance = 13;
  EXPECT_DOUBLE_EQ(pe2d::math::SquaredDistance(p1, p2), distance);
}

TEST(MathTest, Perpendicular) {
  const pe2d::Vec2d v1(1.0, 2.0);
  const pe2d::Vec2d v1_perp(-2.0, 1.0);
  EXPECT_EQ(pe2d::math::Perp(v1), v1_perp);
}

TEST(MathTest, Normalize) {
  const pe2d::Vec2d v1(10.0, 5.0);
  const pe2d::Vec2d v1_normalized(0.894427190999916, 0.447213595499958);
  EXPECT_DOUBLE_EQ(v1_normalized.x, pe2d::math::Normalize(v1).x);
  EXPECT_DOUBLE_EQ(v1_normalized.y, pe2d::math::Normalize(v1).y);
}

TEST(MathTest, DotProduct) {
  const pe2d::Vec2d v1(10.0, 2.0);
  const pe2d::Vec2d v2(2.0, 5.0);
  const double dot = 30.0;
  EXPECT_DOUBLE_EQ(pe2d::math::Dot(v1, v2), dot);
}

TEST(MathTest, CrossProduct) {
  const pe2d::Vec2d v1(10.0, 2.0);
  const pe2d::Vec2d v2(2.0, 5.0);
  const double cross = 46.0;
  EXPECT_DOUBLE_EQ(pe2d::math::Cross(v1, v2), cross);
}
} // namespace