// local
#include "collision_points.hpp"

// lib
// gtest
#include <gtest/gtest.h>

namespace {
TEST(CollisionPointsTest, DefaultConstructor) {
  pe2d::CollisionPoints collision_points;
  const pe2d::Vec2d zero_vec = pe2d::Vec2d(0.0, 0.0);
  EXPECT_EQ(collision_points.Normal, zero_vec);
  EXPECT_EQ(collision_points.ContactPoint1, zero_vec);
  EXPECT_EQ(collision_points.ContactPoint2, zero_vec);
  EXPECT_DOUBLE_EQ(collision_points.Depth, 0.0f);
  EXPECT_EQ(collision_points.ContactCount, 0);
  EXPECT_EQ(collision_points.HasCollision, false);
}

TEST(CollisionPointsTest, TwoContactPointsConstructor) {
  const pe2d::Vec2d normal = pe2d::Vec2d(0.5, 0.5);
  const pe2d::Pos2d contact_point1 = pe2d::Pos2d(1.0, 1.0);
  const pe2d::Pos2d contact_point2 = pe2d::Pos2d(2.0, 2.0);
  const float depth = 0.5;
  const bool has_collision = true;

  pe2d::CollisionPoints collision_points(normal, depth, contact_point1,
                                         contact_point2, has_collision);
  EXPECT_EQ(collision_points.Normal, normal);
  EXPECT_EQ(collision_points.ContactPoint1, contact_point1);
  EXPECT_EQ(collision_points.ContactPoint2, contact_point2);
  EXPECT_DOUBLE_EQ(collision_points.Depth, depth);
  EXPECT_EQ(collision_points.ContactCount, 2);
  EXPECT_EQ(collision_points.HasCollision, has_collision);
}

TEST(CollisionPointsTest, OneContactPointConstructor) {
  const pe2d::Vec2d normal = pe2d::Vec2d(0.5, 0.5);
  const pe2d::Pos2d contact_point1 = pe2d::Pos2d(1.0, 1.0);
  const float depth = 0.5;
  const bool has_collision = true;

  pe2d::CollisionPoints collision_points(normal, depth, contact_point1,
                                         has_collision);
  EXPECT_EQ(collision_points.Normal, normal);
  EXPECT_EQ(collision_points.ContactPoint1, contact_point1);
  EXPECT_EQ(collision_points.ContactPoint2, pe2d::Pos2d(0.0, 0.0));
  EXPECT_DOUBLE_EQ(collision_points.Depth, depth);
  EXPECT_EQ(collision_points.ContactCount, 1);
  EXPECT_EQ(collision_points.HasCollision, has_collision);
}
} // namespace