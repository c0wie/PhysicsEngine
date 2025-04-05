// local
#include "math.hpp"
#include "vector2.hpp"

// lib
// gtest
#include <gtest/gtest.h>

namespace {
using namespace pe2d;
TEST(MathTest, Length) {
  const Vec2f v1(-4.0, 4.0);
  const float v1_length = 5.65685424949238;
  EXPECT_FLOAT_EQ(math::Length(v1), v1_length);
}

TEST(MathTest, SquaredLength) {
  const Vec2f v1(-4.0, 4.0);
  const float v1_length_squared = 32;
  EXPECT_FLOAT_EQ(math::SquaredLength(v1), v1_length_squared);
}

TEST(MathTest, Distance) {
  const Vec2f p1(1.0, 1.0);
  const Vec2f p2(3.0, 4.0);
  const float distance = 3.605551275463989;
  EXPECT_FLOAT_EQ(math::Distance(p1, p2), distance);
}

TEST(MathTest, DistanceSquared) {
  const Vec2f p1(1.0, 1.0);
  const Vec2f p2(3.0, 4.0);
  const float distance = 13;
  EXPECT_FLOAT_EQ(math::SquaredDistance(p1, p2), distance);
}

TEST(MathTest, Perpendicular) {
  const Vec2f v1(1.0, 2.0);
  const Vec2f v1_perp(-2.0, 1.0);
  EXPECT_EQ(math::Perp(v1), v1_perp);
}

TEST(MathTest, Normalize) {
  const Vec2f v1(10.0, 5.0);
  const Vec2f v1_normalized(0.894427190999916, 0.447213595499958);
  EXPECT_FLOAT_EQ(v1_normalized.x, math::Normalize(v1).x);
  EXPECT_FLOAT_EQ(v1_normalized.y, math::Normalize(v1).y);
}

TEST(MathTest, DotProduct) {
  const Vec2f v1(10.0, 2.0);
  const Vec2f v2(2.0, 5.0);
  const float dot = 30.0;
  EXPECT_FLOAT_EQ(math::Dot(v1, v2), dot);
}

TEST(MathTest, CrossProduct) {
  const Vec2f v1(10.0, 2.0);
  const Vec2f v2(2.0, 5.0);
  const float cross = 46.0;
  EXPECT_FLOAT_EQ(math::Cross(v1, v2), cross);
}
} // namespace