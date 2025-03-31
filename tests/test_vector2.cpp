// local
#include "vector2.hpp"

// lib
// gtest
#include <gtest/gtest.h>
namespace {
using namespace pe2d;
TEST(Vector2Test, DefaultConstructor) {
  Vector2<double> v1;
  EXPECT_DOUBLE_EQ(0.0, v1.x);
  EXPECT_DOUBLE_EQ(0.0, v1.y);
}

TEST(Vector2Test, Constructor) {
  Vector2<double> v1(69.69, 2115.2115);
  EXPECT_DOUBLE_EQ(69.69, v1.x);
  EXPECT_DOUBLE_EQ(2115.2115, v1.y);
}

TEST(Vector2Test, EqualityCheck) {
  Vector2<double> v1(1.0, 1.0);
  Vector2<double> v2(1.0, 1.0);
  Vector2<double> v3(4.0, 4.0);
  EXPECT_TRUE(v1 == v2);
  EXPECT_FALSE(v1 == v3);
}

TEST(Vector2Test, InequalityCheck) {
  Vector2<double> v1(1.0, 1.0);
  Vector2<double> v2(1.0, 1.0);
  Vector2<double> v3(1.0, 4.0);
  EXPECT_FALSE(v1 != v2);
  EXPECT_TRUE(v1 != v3);
}

TEST(Vector2Test, Addition) {
  Vector2<double> v1(2.0, 2.0);
  Vector2<double> v2(3.0, 3.0);
  Vector2<double> v3 = v1 + v2;
  EXPECT_DOUBLE_EQ(v3.x, 5.0);
  EXPECT_DOUBLE_EQ(v3.y, 5.0);

  Vector2<double> v4;
  v4 += v1;
  EXPECT_DOUBLE_EQ(v4.x, 2.0);
  EXPECT_DOUBLE_EQ(v4.y, 2.0);
}

TEST(Vector2Test, Subtraction) {
  Vector2<double> v1(2.0, 2.0);
  Vector2<double> v2(3.0, 3.0);
  Vector2<double> v3 = v1 - v2;
  EXPECT_DOUBLE_EQ(v3.x, -1.0);
  EXPECT_DOUBLE_EQ(v3.y, -1.0);

  Vector2<double> v4;
  v4 -= v1;
  EXPECT_DOUBLE_EQ(v4.x, -2.0);
  EXPECT_DOUBLE_EQ(v4.y, -2.0);
}

TEST(Vector2Test, VectorMultiplication) {
  Vector2<double> v1(2.0, 2.0);
  Vector2<double> v2(2.0, 3.0);
  Vector2<double> v3 = v1 * v2;
  EXPECT_DOUBLE_EQ(v3.x, 4.0);
  EXPECT_DOUBLE_EQ(v3.y, 6.0);
}

TEST(Vector2Test, ScalarMultiplication) {
  Vector2<double> v1(1.0, 1.0);
  Vector2<double> v2 = v1 * 2.0;
  EXPECT_DOUBLE_EQ(v2.x, 2.0);
  EXPECT_DOUBLE_EQ(v2.y, 2.0);

  Vector2<double> v3(4.0, 6.0);
  v3 *= 2;
  EXPECT_DOUBLE_EQ(v3.x, 8.0);
  EXPECT_DOUBLE_EQ(v3.y, 12.0);
}

TEST(Vector2Test, VectorDivision) {
  Vector2<double> v1(6.0, 4.0);
  Vector2<double> v2(2.0, 2.0);
  Vector2<double> v3 = v1 / v2;
  EXPECT_DOUBLE_EQ(v3.x, 3.0);
  EXPECT_DOUBLE_EQ(v3.y, 2.0);
}

TEST(Vector2Test, ScalarDivision) {
  Vector2<double> v1(2.0, 2.0);
  Vector2<double> v2 = v1 / 2.0;
  EXPECT_DOUBLE_EQ(v2.x, 1.0);
  EXPECT_DOUBLE_EQ(v2.y, 1.0);

  Vector2<double> v3(4.0, 4.0);
  v3 /= 2.0;
  EXPECT_DOUBLE_EQ(v3.x, 2.0);
  EXPECT_DOUBLE_EQ(v3.y, 2.0);
}
} // namespace