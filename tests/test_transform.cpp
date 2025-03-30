// local
#include "angle.hpp"
#include "transform.hpp"

// lib
// gtest
#include <gtest/gtest.h>

namespace {
using namespace pe2d;
constexpr double DOUBLE_INACCURACY = 1e-12;
TEST(TansformTest, DefaultConstructor) {
  Transform transform;
  EXPECT_EQ(transform.position, Vec2d(0.0, 0.0));
  EXPECT_EQ(transform.scale, Vec2d(1.0, 1.0));
  EXPECT_EQ(transform.angle, Angle());
}

TEST(TransformTest, FullConstructor) {
  const Vec2d position = Vec2d(10.0, 10.0);
  const Vec2d scale = Vec2d(2.0, 1.5);
  const Angle angle = Angle::FromDegrees(45.0);
  Transform transform(position, angle, scale);
  EXPECT_EQ(transform.position, position);
  EXPECT_EQ(transform.scale, scale);
  EXPECT_EQ(transform.angle, Angle::FromDegrees(45.0));
}

TEST(TransformTest, PosRotationConstructor) {
  const Vec2d position = Vec2d(10.0, 10.0);
  const Angle angle = Angle::FromDegrees(45.0);
  Transform transform(position, angle);
  EXPECT_EQ(transform.position, position);
  EXPECT_EQ(transform.scale, Vec2d(1.0, 1.0));
  EXPECT_EQ(transform.angle, Angle::FromDegrees(45.0));
}

TEST(TransformTest, PosConstructor) {
  const Vec2d position = Vec2d(10.0, 10.0);
  Transform transform(position);
  EXPECT_EQ(transform.position, position);
  EXPECT_EQ(transform.scale, Vec2d(1.0, 1.0));
  EXPECT_EQ(transform.angle, Angle());
}

TEST(TransformTest, Move) {
  const Vec2d position = Vec2d(10.0, 10.0);
  const Vec2d offset = Vec2d(5.0, 5.0);
  const Vec2d new_position = position + offset;
  Transform transform(position);

  transform.Move(offset);
  EXPECT_EQ(transform.position, new_position);
}

TEST(TransformTest, Rotate) {
  const Vec2d position = Vec2d(10.0, 10.0);
  Transform transform(position);
  const Angle angle = Angle::FromDegrees(45.0);
  transform.Rotate(angle);
  EXPECT_EQ(transform.angle, Angle::FromDegrees(45.0));
}
} // namespace