// local
#include "vector2.hpp"

// lib
// gtest
#include <gtest/gtest.h>
namespace {

TEST(Vector2Test, DefaultConstructor) {
  pe2d::Vector2<double> v1;
  EXPECT_DOUBLE_EQ(0.0, v1.x);
  EXPECT_DOUBLE_EQ(0.0, v1.y);
}

TEST(Vector2Test, Constructor) {
  pe2d::Vector2<double> v1(69.69, 2115.2115);
  EXPECT_DOUBLE_EQ(69.69, v1.x);
  EXPECT_DOUBLE_EQ(2115.2115, v1.y);
}

TEST(Vector2Test, EqualityCheck) {
  pe2d::Vector2<double> v1(1.0, 1.0);
  pe2d::Vector2<double> v2(1.0, 1.0);
  pe2d::Vector2<double> v3(4.0, 4.0);
  EXPECT_TRUE(v1 == v2);
  EXPECT_FALSE(v1 == v3);
}

TEST(Vector2Test, InequalityCheck) {
  pe2d::Vector2<double> v1(1.0, 1.0);
  pe2d::Vector2<double> v2(1.0, 1.0);
  pe2d::Vector2<double> v3(4.0, 4.0);
  EXPECT_FALSE(v1 != v2);
  EXPECT_TRUE(v1 != v3);
}

TEST(Vector2Test, Addition) {
  pe2d::Vector2<double> v1(2.0, 2.0);
  pe2d::Vector2<double> v2(3.0, 3.0);
  pe2d::Vector2<double> v3 = v1 + v2;
  EXPECT_DOUBLE_EQ(v3.x, 5.0);
  EXPECT_DOUBLE_EQ(v3.y, 5.0);

  pe2d::Vector2<double> v4;
  v4 += v1;
  EXPECT_DOUBLE_EQ(v4.x, 2.0);
  EXPECT_DOUBLE_EQ(v4.y, 2.0);
}

TEST(Vector2Test, Subtraction) {
  pe2d::Vector2<double> v1(2.0, 2.0);
  pe2d::Vector2<double> v2(3.0, 3.0);
  pe2d::Vector2<double> v3 = v1 - v2;
  EXPECT_DOUBLE_EQ(v3.x, -1.0);
  EXPECT_DOUBLE_EQ(v3.y, -1.0);

  pe2d::Vector2<double> v4;
  v4 -= v1;
  EXPECT_DOUBLE_EQ(v4.x, -2.0);
  EXPECT_DOUBLE_EQ(v4.y, -2.0);
}

TEST(Vector2Test, VectorMultiplication) {
  pe2d::Vector2<double> v1(2.0, 2.0);
  pe2d::Vector2<double> v2(2.0, 3.0);
  pe2d::Vector2<double> v3 = v1 * v2;
  EXPECT_DOUBLE_EQ(v3.x, 4.0);
  EXPECT_DOUBLE_EQ(v3.y, 6.0);
}

TEST(Vector2Test, ScalarMultiplication) {
  pe2d::Vector2<double> v1(1.0, 1.0);
  pe2d::Vector2<double> v2 = v1 * 2.0;
  EXPECT_DOUBLE_EQ(v2.x, 2.0);
  EXPECT_DOUBLE_EQ(v2.y, 2.0);

  pe2d::Vector2<double> v3(4.0, 6.0);
  v3 *= 2;
  EXPECT_DOUBLE_EQ(v3.x, 8.0);
  EXPECT_DOUBLE_EQ(v3.y, 12.0);
}

TEST(Vector2Test, VectorDivision) {
  pe2d::Vector2<double> v1(6.0, 4.0);
  pe2d::Vector2<double> v2(2.0, 2.0);
  pe2d::Vector2<double> v3 = v1 / v2;
  EXPECT_DOUBLE_EQ(v3.x, 3.0);
  EXPECT_DOUBLE_EQ(v3.y, 2.0);
}

TEST(Vector2Test, ScalarDivision) {
  pe2d::Vector2<double> v1(2.0, 2.0);
  pe2d::Vector2<double> v2 = v1 / 2.0;
  EXPECT_DOUBLE_EQ(v2.x, 1.0);
  EXPECT_DOUBLE_EQ(v2.y, 1.0);

  pe2d::Vector2<double> v3(4.0, 4.0);
  v3 /= 2.0;
  EXPECT_DOUBLE_EQ(v3.x, 2.0);
  EXPECT_DOUBLE_EQ(v3.y, 2.0);
}
} // namespace