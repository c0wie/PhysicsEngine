// local
#include "collision_points.hpp"

// lib
// gtest
#include <gtest/gtest.h>

namespace {
using namespace pe2d;
TEST(CollisionPointsTest, DefaultConstructor) {
  CollisionPoints collision_points;
  const Vec2d zero_vec = Vec2d(0.0, 0.0);
  EXPECT_EQ(collision_points.normal, zero_vec);
  EXPECT_EQ(collision_points.contact_point1, zero_vec);
  EXPECT_EQ(collision_points.contact_point2, zero_vec);
  EXPECT_DOUBLE_EQ(collision_points.depth, 0.0f);
  EXPECT_EQ(collision_points.contact_count, 0);
}

TEST(CollisionPointsTest, TwoContactPointsConstructor) {
  const Vec2d normal = Vec2d(0.5, 0.5);
  const Pos2d contact_point1 = Pos2d(1.0, 1.0);
  const Pos2d contact_point2 = Pos2d(2.0, 2.0);
  const float depth = 0.5;

  CollisionPoints collision_points(normal, depth, contact_point1,
                                   contact_point2);
  EXPECT_EQ(collision_points.normal, normal);
  EXPECT_EQ(collision_points.contact_point1, contact_point1);
  EXPECT_EQ(collision_points.contact_point2, contact_point2);
  EXPECT_DOUBLE_EQ(collision_points.depth, depth);
  EXPECT_EQ(collision_points.contact_count, 2);
}

TEST(CollisionPointsTest, OneContactPointConstructor) {
  const Vec2d normal = Vec2d(0.5, 0.5);
  const Pos2d contact_point1 = Pos2d(1.0, 1.0);
  const float depth = 0.5;

  CollisionPoints collision_points(normal, depth, contact_point1);
  EXPECT_EQ(collision_points.normal, normal);
  EXPECT_EQ(collision_points.contact_point1, contact_point1);
  EXPECT_EQ(collision_points.contact_point2, Pos2d(0.0, 0.0));
  EXPECT_DOUBLE_EQ(collision_points.depth, depth);
  EXPECT_EQ(collision_points.contact_count, 1);
}
} // namespace