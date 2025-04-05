// local
#include "angle.hpp"
#include "transform.hpp"
#include "vector2.hpp"

// lib
// gtest
#include <gtest/gtest.h>

namespace {
using namespace pe2d;
TEST(TansformTest, DefaultConstructor) {
  Transform transform;
  EXPECT_EQ(transform.position, Vec2f(0.0, 0.0));
  EXPECT_EQ(transform.scale, Vec2f(1.0, 1.0));
  EXPECT_EQ(transform.angle, Angle());
}

TEST(TransformTest, FullConstructor) {
  const Vec2f position = Vec2f(10.0, 10.0);
  const Vec2f scale = Vec2f(2.0, 1.5);
  const Angle angle = Angle::FromDegrees(45.0);
  Transform transform(position, angle, scale);
  EXPECT_EQ(transform.position, position);
  EXPECT_EQ(transform.scale, scale);
  EXPECT_EQ(transform.angle, Angle::FromDegrees(45.0));
}

TEST(TransformTest, PosRotationConstructor) {
  const Vec2f position = Vec2f(10.0, 10.0);
  const Angle angle = Angle::FromDegrees(45.0);
  Transform transform(position, angle);
  EXPECT_EQ(transform.position, position);
  EXPECT_EQ(transform.scale, Vec2f(1.0, 1.0));
  EXPECT_EQ(transform.angle, Angle::FromDegrees(45.0));
}

TEST(TransformTest, PosConstructor) {
  const Vec2f position = Vec2f(10.0, 10.0);
  Transform transform(position);
  EXPECT_EQ(transform.position, position);
  EXPECT_EQ(transform.scale, Vec2f(1.0, 1.0));
  EXPECT_EQ(transform.angle, Angle());
}

TEST(TransformTest, Move) {
  const Vec2f position = Vec2f(10.0, 10.0);
  const Vec2f offset = Vec2f(5.0, 5.0);
  const Vec2f new_position = position + offset;
  Transform transform(position);

  transform.Move(offset);
  EXPECT_EQ(transform.position, new_position);
}

TEST(TransformTest, Rotate) {
  const Vec2f position = Vec2f(10.0, 10.0);
  Transform transform(position);
  const Angle angle = Angle::FromDegrees(45.0);
  transform.Rotate(angle);
  EXPECT_EQ(transform.angle, Angle::FromDegrees(45.0));
}
} // namespace