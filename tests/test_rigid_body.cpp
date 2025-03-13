#include "math.hpp"
#include "rigid_body.hpp"
#include "transform.hpp"
#include "vector2.hpp"
#include "gtest/gtest.h"
#include <cstddef>
#include <gtest/gtest.h>

namespace {
TEST(RigidBodyTest, DefaultConstructor) {
  pe2d::RigidBody rigid_body;
  const pe2d::Vec2d zero_vec = pe2d::Vec2d(0.0, 0.0);
  EXPECT_EQ(rigid_body.GetType(), 0);
  EXPECT_EQ(rigid_body.GetID(), 0);
  EXPECT_FALSE(rigid_body.IsStatic());
  EXPECT_DOUBLE_EQ(rigid_body.GetMass(), 0.0);
  EXPECT_EQ(rigid_body.GetSize(), zero_vec);
  EXPECT_EQ(rigid_body.GetLinearVelocity(), zero_vec);
  EXPECT_DOUBLE_EQ(rigid_body.GetAngularVelocity(), 0.0);
  EXPECT_DOUBLE_EQ(rigid_body.GetStaticFriction(), 0.0);
  EXPECT_DOUBLE_EQ(rigid_body.GetDynamicFriction(), 0.0);
  EXPECT_DOUBLE_EQ(rigid_body.GetRestitution(), 0.0);
  EXPECT_EQ(rigid_body.GetForce(), zero_vec);
  EXPECT_EQ(rigid_body.GetGravity(), zero_vec);
  EXPECT_EQ(rigid_body.GetPosition(), zero_vec);
  EXPECT_DOUBLE_EQ(rigid_body.GetRotation(), 0.0);
  EXPECT_EQ(rigid_body.GetScale(), pe2d::Vec2d(1.0, 1.0));
}

TEST(RigidBodyTest, FullConstructor) {
  const pe2d::Vec2d zero_vec = pe2d::Vec2d(0.0, 0.0);
  const pe2d::RigidBodyType type = pe2d::RigidBodyType::Box;
  const std::size_t id = 1;
  const pe2d::Size2d size(10.0, 10.0);
  const pe2d::Vec2d pos(10.0, 10.0);
  const double rotation(pe2d::math::DeegresToRadians(45.0));
  const pe2d::Vec2d scale(2.0, 0.5);
  const double mass = 100.0;
  const bool is_static = true;
  const pe2d::Vec2d gravity(0.0, 9.81);
  const pe2d::Vec2d linear_velocity(100.0, 0.0);
  const double angular_velocity = 2.0;
  const double static_friction = 0.5;
  const double dynamic_friction = 0.37;
  const double restitution = 0.2;
  pe2d::RigidBody rigid_body(
      id, type, size, pe2d::Transform(pos, rotation, scale), mass, is_static,
      gravity, linear_velocity, angular_velocity, static_friction,
      dynamic_friction, restitution);
  EXPECT_EQ(rigid_body.GetType(), type);
  EXPECT_EQ(rigid_body.GetID(), id);
  EXPECT_TRUE(rigid_body.IsStatic());
  EXPECT_DOUBLE_EQ(rigid_body.GetMass(), pe2d::math::INF);
  EXPECT_EQ(rigid_body.GetSize(), size);
  EXPECT_EQ(rigid_body.GetLinearVelocity(), linear_velocity);
  EXPECT_DOUBLE_EQ(rigid_body.GetAngularVelocity(), angular_velocity);
  EXPECT_DOUBLE_EQ(rigid_body.GetStaticFriction(), static_friction);
  EXPECT_DOUBLE_EQ(rigid_body.GetDynamicFriction(), dynamic_friction);
  EXPECT_DOUBLE_EQ(rigid_body.GetRestitution(), restitution);
  EXPECT_EQ(rigid_body.GetForce(), zero_vec);
  EXPECT_EQ(rigid_body.GetGravity(), gravity);
  EXPECT_EQ(rigid_body.GetPosition(), pos);
  EXPECT_DOUBLE_EQ(rigid_body.GetRotation(), rotation);
  EXPECT_EQ(rigid_body.GetScale(), scale);
}

TEST(RigidBodyTest, NoMotionNoFrictionConstructor) {
  const pe2d::Vec2d zero_vec = pe2d::Vec2d(0.0, 0.0);
  const pe2d::RigidBodyType type = pe2d::RigidBodyType::Box;
  const std::size_t id = 1;
  const pe2d::Size2d size(10.0, 10.0);
  const pe2d::Vec2d pos(10.0, 10.0);
  const double rotation(pe2d::math::DeegresToRadians(45.0));
  const pe2d::Vec2d scale(2.0, 0.5);
  const double mass = 100.0;
  const bool is_static = false;
  const pe2d::Vec2d gravity(0.0, 9.81);
  pe2d::RigidBody rigid_body(id, type, size,
                             pe2d::Transform(pos, rotation, scale), mass,
                             is_static, gravity);
  EXPECT_EQ(rigid_body.GetType(), type);
  EXPECT_EQ(rigid_body.GetID(), id);
  EXPECT_FALSE(rigid_body.IsStatic());
  EXPECT_DOUBLE_EQ(rigid_body.GetMass(), mass);
  EXPECT_EQ(rigid_body.GetSize(), size);
  EXPECT_EQ(rigid_body.GetLinearVelocity(), zero_vec);
  EXPECT_DOUBLE_EQ(rigid_body.GetAngularVelocity(), 0.0);
  EXPECT_DOUBLE_EQ(rigid_body.GetStaticFriction(), 0.0);
  EXPECT_DOUBLE_EQ(rigid_body.GetDynamicFriction(), 0.0);
  EXPECT_DOUBLE_EQ(rigid_body.GetRestitution(), 0.0);
  EXPECT_EQ(rigid_body.GetForce(), zero_vec);
  EXPECT_EQ(rigid_body.GetGravity(), gravity);
  EXPECT_EQ(rigid_body.GetPosition(), pos);
  EXPECT_DOUBLE_EQ(rigid_body.GetRotation(), rotation);
  EXPECT_EQ(rigid_body.GetScale(), scale);
}

TEST(RigidBodyTest, NoFrictionConstructor) {
  const pe2d::Vec2d zero_vec = pe2d::Vec2d(0.0, 0.0);
  const pe2d::RigidBodyType type = pe2d::RigidBodyType::Box;
  const std::size_t id = 1;
  const pe2d::Size2d size(10.0, 10.0);
  const pe2d::Vec2d pos(10.0, 10.0);
  const double rotation(pe2d::math::DeegresToRadians(45.0));
  const pe2d::Vec2d scale(2.0, 0.5);
  const double mass = 100.0;
  const bool is_static = false;
  const pe2d::Vec2d gravity(0.0, 9.81);
  const pe2d::Vec2d linear_velocity(100.0, 0.0);
  const double angular_velocity = 2.0;
  pe2d::RigidBody rigid_body(
      id, type, size, pe2d::Transform(pos, rotation, scale), mass, is_static,
      gravity, linear_velocity, angular_velocity);
  EXPECT_EQ(rigid_body.GetType(), type);
  EXPECT_EQ(rigid_body.GetID(), id);
  EXPECT_FALSE(rigid_body.IsStatic());
  EXPECT_DOUBLE_EQ(rigid_body.GetMass(), mass);
  EXPECT_EQ(rigid_body.GetSize(), size);
  EXPECT_EQ(rigid_body.GetLinearVelocity(), linear_velocity);
  EXPECT_DOUBLE_EQ(rigid_body.GetAngularVelocity(), angular_velocity);
  EXPECT_DOUBLE_EQ(rigid_body.GetStaticFriction(), 0.0);
  EXPECT_DOUBLE_EQ(rigid_body.GetDynamicFriction(), 0.0);
  EXPECT_DOUBLE_EQ(rigid_body.GetRestitution(), 0.0);
  EXPECT_EQ(rigid_body.GetForce(), zero_vec);
  EXPECT_EQ(rigid_body.GetGravity(), gravity);
  EXPECT_EQ(rigid_body.GetPosition(), pos);
  EXPECT_DOUBLE_EQ(rigid_body.GetRotation(), rotation);
  EXPECT_EQ(rigid_body.GetScale(), scale);
}

TEST(RigidBodyTest, BoxSetSize) {
  const pe2d::Size2d new_size(10.0, 4.0);
  pe2d::RigidBody box_rigid_body(1, pe2d::RigidBodyType::Box,
                                 pe2d::Size2d(10.0, 10.0), {}, 10.0, false, {});
  box_rigid_body.SetSize(new_size);
  EXPECT_EQ(box_rigid_body.GetSize(), new_size);
}

TEST(RigidBodyTest, CircleSetSize) {
  const pe2d::Size2d new_size(10.0, 4.0);
  pe2d::RigidBody circle_rigid_body(2, pe2d::RigidBodyType::Circle,
                                    pe2d::Size2d(10.0, 10.0), {}, 10.0, false,
                                    {});
  circle_rigid_body.SetSize(new_size);
  EXPECT_EQ(circle_rigid_body.GetSize(), pe2d::Size2d(new_size.x, new_size.x));
}

TEST(RigidBodyTest, BoxSetScale) {
  const pe2d::Size2d new_scale(2.0, 4.0);
  pe2d::RigidBody box_rigid_body(1, pe2d::RigidBodyType::Box,
                                 pe2d::Size2d(10.0, 10.0), {}, 10.0, false, {});
  box_rigid_body.SetScale(new_scale);
  EXPECT_EQ(box_rigid_body.GetScale(), new_scale);
}

TEST(RigidBodyTest, CircleSetScale) {
  const pe2d::Size2d new_scale(10.0, 4.0);
  pe2d::RigidBody circle_rigid_body(2, pe2d::RigidBodyType::Circle,
                                    pe2d::Size2d(10.0, 10.0), {}, 10.0, false,
                                    {});
  circle_rigid_body.SetScale(new_scale);
  EXPECT_EQ(circle_rigid_body.GetScale(),
            pe2d::Size2d(new_scale.x, new_scale.x));
}

TEST(RigidBodyTest, BoxInertia) {
  const pe2d::RigidBody box_rigid_body(2, pe2d::RigidBodyType::Box,
                                       pe2d::Size2d(10.0, 10.0), {}, 10.0,
                                       false, {});
  const double inertia = 166.0;
  EXPECT_DOUBLE_EQ(box_rigid_body.GetRotationalInertia(), inertia);
}

TEST(RigidBodyTest, CircleInertia) {
  const pe2d::RigidBody circle_rigid_body(2, pe2d::RigidBodyType::Circle,
                                          pe2d::Size2d(10.0, 10.0), {}, 10.0,
                                          false, {});
  const double inertia = 500;
  EXPECT_DOUBLE_EQ(circle_rigid_body.GetRotationalInertia(), inertia);
}

TEST(RigigObjectTest, CircleGetBoundingBox) {
  const pe2d::RigidBody circle_rigid_body(2, pe2d::RigidBodyType::Circle,
                                          pe2d::Size2d(10.0, 10.0), {}, 10.0,
                                          false, {});

  const std::array<pe2d::Pos2d, 4> expected_vertieces = {
      pe2d::Pos2d(-10.0, -10.0), pe2d::Pos2d(10.0, -10.0),
      pe2d::Pos2d(10.0, 10.0), pe2d::Pos2d(-10.0, 10.0)};

  const std::array<pe2d::Pos2d, 4> vertecies =
      circle_rigid_body.GetBoundingBox();
  for (std::size_t i = 0; i < expected_vertieces.size(); i++) {
    EXPECT_EQ(vertecies[i], expected_vertieces[i]);
  }
}

TEST(RigigObjectTest, BoxGetBoundingBox) {
  const pe2d::RigidBody box_rigid_body(2, pe2d::RigidBodyType::Box,
                                       pe2d::Size2d(10.0, 10.0), {}, 10.0,
                                       false, {});

  const std::array<pe2d::Pos2d, 4> expected_vertieces = {
      pe2d::Pos2d(-5.0, -5.0), pe2d::Pos2d(5.0, -5.0), pe2d::Pos2d(5.0, 5.0),
      pe2d::Pos2d(-5.0, 5.0)};

  const std::array<pe2d::Pos2d, 4> vertecies = box_rigid_body.GetBoundingBox();
  for (std::size_t i = 0; i < expected_vertieces.size(); i++) {
    EXPECT_EQ(vertecies[i], expected_vertieces[i]);
  }
}

TEST(RigigObjectTest, RotatedBoxGetBoundingBox) {
  const pe2d::RigidBody box_rigid_body(
      2, pe2d::RigidBodyType::Box, pe2d::Size2d(10.0, 10.0),
      {{0.0, 0.0}, pe2d::math::DeegresToRadians(45.0)}, 10.0, false, {});

  const std::array<pe2d::Pos2d, 4> expected_vertieces = {
      pe2d::Pos2d(-7.0710676908493042, -7.0710676908493042), pe2d::Pos2d(7.0710676908493042, -7.0710676908493042),
      pe2d::Pos2d(7.0710676908493042, 7.0710676908493042),
      pe2d::Pos2d(-7.0710676908493042, 7.0710676908493042)};

  const std::array<pe2d::Pos2d, 4> vertecies = box_rigid_body.GetBoundingBox();
  for (std::size_t i = 0; i < expected_vertieces.size(); i++) {
    EXPECT_DOUBLE_EQ(vertecies[i].x, expected_vertieces[i].x);
    EXPECT_DOUBLE_EQ(vertecies[i].y, expected_vertieces[i].y);
  }
}
} // namespace