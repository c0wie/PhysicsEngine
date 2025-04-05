// local
#include "angle.hpp"
// gtest
#include <gtest/gtest.h>


namespace {
using namespace pe2d;
TEST(AngleTest, DefaultConstructor) {
  const Angle angle;
  EXPECT_FLOAT_EQ(angle.AsDegrees(), 0.0);
  EXPECT_FLOAT_EQ(angle.AsRadians(), 0.0);
}
TEST(AngleTest, DegreesConstructor) {
  const Angle angle = Angle::FromDegrees(45.0);
  EXPECT_FLOAT_EQ(angle.AsDegrees(), 45.0);
  EXPECT_FLOAT_EQ(angle.AsRadians(), 0.785398163397435);
}
TEST(AngleTest, RadiansConstructor) {
  const Angle angle = Angle::FromRadians(0.785398163397435);
  EXPECT_FLOAT_EQ(angle.AsDegrees(), 45.0);
  EXPECT_FLOAT_EQ(angle.AsRadians(), 0.785398163397435);
}
}