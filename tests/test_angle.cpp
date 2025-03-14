// local
#include "angle.hpp"
// gtest
#include <gtest/gtest.h>


namespace {
  constexpr double DOUBLE_INACCURACY = 1e-12;
  TEST(AngleTest, DefaultConstructor) {
    const pe2d::Angle angle;
    EXPECT_DOUBLE_EQ(angle.AsDegrees(), 0.0);
    EXPECT_DOUBLE_EQ(angle.AsRadians(), 0.0);
  }

  TEST(AngleTest, DegreesConstructor) {
    const pe2d::Angle angle = pe2d::Angle::FromDegrees(45.0);
    EXPECT_NEAR(angle.AsDegrees(), 45.0, DOUBLE_INACCURACY);
    EXPECT_NEAR(angle.AsRadians(), 0.785398163397435, DOUBLE_INACCURACY);
  }

  TEST(AngleTest, RadiansConstructor) {
    const pe2d::Angle angle = pe2d::Angle::FromRadians(0.785398163397435);
    EXPECT_NEAR(angle.AsDegrees(), 45.0, DOUBLE_INACCURACY);
    EXPECT_NEAR(angle.AsRadians(), 0.785398163397435, DOUBLE_INACCURACY);
  }
}