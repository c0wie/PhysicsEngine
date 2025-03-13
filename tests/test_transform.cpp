// local
#include "math.hpp"
#include "transform.hpp"

// lib
// gtest
#include <gtest/gtest.h>

namespace {
TEST(TansformTest, DefaultConstructor) {
  pe2d::Transform transform;
  EXPECT_EQ(transform.position, pe2d::Vec2d(0.0, 0.0));
  EXPECT_EQ(transform.scale, pe2d::Vec2d(1.0, 1.0));
  EXPECT_DOUBLE_EQ(transform.rotation, 0.0);
}

TEST(TransformTest, FullConstructor) {
  const pe2d::Vec2d position = pe2d::Vec2d(10.0, 10.0);
  const pe2d::Vec2d scale = pe2d::Vec2d(2.0, 1.5);
  const double rotation = pe2d::math::DeegresToRadians(45.0);
  pe2d::Transform transform(position, rotation, scale);
  EXPECT_EQ(transform.position, position);
  EXPECT_EQ(transform.scale, scale);
  EXPECT_DOUBLE_EQ(transform.rotation, rotation);
}

TEST(TransformTest, PosRotationConstructor) {
  const pe2d::Vec2d position = pe2d::Vec2d(10.0, 10.0);
  const double rotation = pe2d::math::DeegresToRadians(45.0);
  pe2d::Transform transform(position, rotation);
  EXPECT_EQ(transform.position, position);
  EXPECT_EQ(transform.scale, pe2d::Vec2d(1.0, 1.0));
  EXPECT_DOUBLE_EQ(transform.rotation, rotation);
}

TEST(TransformTest, PosConstructor) {
  const pe2d::Vec2d position = pe2d::Vec2d(10.0, 10.0);
  pe2d::Transform transform(position);
  EXPECT_EQ(transform.position, position);
  EXPECT_EQ(transform.scale, pe2d::Vec2d(1.0, 1.0));
  EXPECT_DOUBLE_EQ(transform.rotation, 0.0);
}

TEST(TransformTest, Move) {
  const pe2d::Vec2d position = pe2d::Vec2d(10.0, 10.0);
  const pe2d::Vec2d offset = pe2d::Vec2d(5.0, 5.0);
  const pe2d::Vec2d new_position = position + offset;
  pe2d::Transform transform(position);

  transform.Move(offset);
  EXPECT_EQ(transform.position, new_position);
}

TEST(TransformTest, Rotate) {
  const pe2d::Vec2d position = pe2d::Vec2d(10.0, 10.0);
  pe2d::Transform transform(position);
  const double rotation = pe2d::math::DeegresToRadians(45.0);
  transform.Rotate(rotation);
  EXPECT_EQ(transform.rotation, rotation);
}
} // namespace