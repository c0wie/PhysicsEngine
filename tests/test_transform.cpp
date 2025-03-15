// local
#include "angle.hpp"
#include "transform.hpp"

// lib
// gtest
#include <gtest/gtest.h>

namespace {
constexpr double DOUBLE_INACCURACY = 1e-12;
TEST(TansformTest, DefaultConstructor) {
  pe2d::Transform transform;
  EXPECT_EQ(transform.position, pe2d::Vec2d(0.0, 0.0));
  EXPECT_EQ(transform.scale, pe2d::Vec2d(1.0, 1.0));
  EXPECT_EQ(transform.angle, pe2d::Angle());
}

TEST(TransformTest, FullConstructor) {
  const pe2d::Vec2d position = pe2d::Vec2d(10.0, 10.0);
  const pe2d::Vec2d scale = pe2d::Vec2d(2.0, 1.5);
  const pe2d::Angle angle = pe2d::Angle::FromDegrees(45.0);
  pe2d::Transform transform(position, angle, scale);
  EXPECT_EQ(transform.position, position);
  EXPECT_EQ(transform.scale, scale);
  EXPECT_EQ(transform.angle, pe2d::Angle::FromDegrees(45.0));
}

TEST(TransformTest, PosRotationConstructor) {
  const pe2d::Vec2d position = pe2d::Vec2d(10.0, 10.0);
  const pe2d::Angle angle = pe2d::Angle::FromDegrees(45.0);
  pe2d::Transform transform(position, angle);
  EXPECT_EQ(transform.position, position);
  EXPECT_EQ(transform.scale, pe2d::Vec2d(1.0, 1.0));
  EXPECT_EQ(transform.angle, pe2d::Angle::FromDegrees(45.0));
}

TEST(TransformTest, PosConstructor) {
  const pe2d::Vec2d position = pe2d::Vec2d(10.0, 10.0);
  pe2d::Transform transform(position);
  EXPECT_EQ(transform.position, position);
  EXPECT_EQ(transform.scale, pe2d::Vec2d(1.0, 1.0));
  EXPECT_EQ(transform.angle, pe2d::Angle());
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
  const pe2d::Angle angle = pe2d::Angle::FromDegrees(45.0);
  transform.Rotate(angle);
  EXPECT_EQ(transform.angle, pe2d::Angle::FromDegrees(45.0));
}
} // namespace