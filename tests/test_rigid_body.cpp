#include "angle.hpp"
#include "math.hpp"
#include "rigid_body.hpp"
#include "transform.hpp"
#include "vector2.hpp"
#include "gtest/gtest.h"
#include <cstddef>
#include <gtest/gtest.h>

namespace {
using namespace pe2d;
TEST(RigidBodyTest, DefaultConstructor) {
  RigidBody rigid_body;
  const Vec2f zero_vec = Vec2f(0.0, 0.0);
  EXPECT_EQ(rigid_body.GetType(), 0);
  EXPECT_EQ(rigid_body.GetID(), 0);
  EXPECT_FALSE(rigid_body.IsStatic());
  EXPECT_FLOAT_EQ(rigid_body.GetMass(), 0.0f);
  EXPECT_EQ(rigid_body.GetSize(), zero_vec);
  EXPECT_EQ(rigid_body.GetLinearVelocity(), zero_vec);
  EXPECT_FLOAT_EQ(rigid_body.GetAngularVelocity(), 0.0);
  EXPECT_FLOAT_EQ(rigid_body.GetStaticFriction(), 0.0);
  EXPECT_FLOAT_EQ(rigid_body.GetDynamicFriction(), 0.0);
  EXPECT_FLOAT_EQ(rigid_body.GetRestitution(), 0.0);
  EXPECT_EQ(rigid_body.GetForce(), zero_vec);
  EXPECT_EQ(rigid_body.GetGravity(), zero_vec);
  EXPECT_EQ(rigid_body.GetPosition(), zero_vec);
  EXPECT_FLOAT_EQ(rigid_body.GetAngle().AsRadians(), 0.0);
  EXPECT_EQ(rigid_body.GetScale(), Vec2f(1.0, 1.0));
}

TEST(RigidBodyTest, FullConstructor) {
  const Vec2f zero_vec = Vec2f(0.0, 0.0);
  const RigidBodyType type = RigidBodyType::Box;
  const std::size_t id = 1;
  const Vec2f size(10.0, 10.0);
  const Vec2f pos(10.0, 10.0);
  const Angle angle = Angle::FromDegrees(45.0);
  const Vec2f scale(2.0, 0.5);
  const float mass = 100.0;
  const bool is_static = true;
  const Vec2f gravity(0.0, 9.81);
  const Vec2f linear_velocity(100.0, 0.0);
  const float angular_velocity = 2.0;
  const float static_friction = 0.5;
  const float dynamic_friction = 0.37;
  const float restitution = 0.2;

  RigidBody rigid_body(id, type, size, Transform(pos, angle, scale), mass,
                       is_static, gravity, linear_velocity, angular_velocity,
                       static_friction, dynamic_friction, restitution);
  EXPECT_EQ(rigid_body.GetType(), type);
  EXPECT_EQ(rigid_body.GetID(), id);
  EXPECT_TRUE(rigid_body.IsStatic());
  EXPECT_FLOAT_EQ(rigid_body.GetMass(), math::INF);
  EXPECT_EQ(rigid_body.GetSize(), size);
  EXPECT_EQ(rigid_body.GetLinearVelocity(), linear_velocity);
  EXPECT_FLOAT_EQ(rigid_body.GetAngularVelocity(), angular_velocity);
  EXPECT_FLOAT_EQ(rigid_body.GetStaticFriction(), static_friction);
  EXPECT_FLOAT_EQ(rigid_body.GetDynamicFriction(), dynamic_friction);
  EXPECT_FLOAT_EQ(rigid_body.GetRestitution(), restitution);
  EXPECT_EQ(rigid_body.GetForce(), zero_vec);
  EXPECT_EQ(rigid_body.GetGravity(), gravity);
  EXPECT_EQ(rigid_body.GetPosition(), pos);
  EXPECT_EQ(rigid_body.GetAngle(), angle);
  EXPECT_EQ(rigid_body.GetScale(), scale);
}

TEST(RigidBodyTest, NoMotionNoFrictionConstructor) {
  const Vec2f zero_vec = Vec2f(0.0, 0.0);
  const RigidBodyType type = RigidBodyType::Box;
  const std::size_t id = 1;
  const Vec2f size(10.0, 10.0);
  const Vec2f pos(10.0, 10.0);
  const Angle angle = Angle::FromDegrees(45.0);
  const Vec2f scale(2.0, 0.5);
  const float mass = 100.0;
  const bool is_static = false;
  const Vec2f gravity(0.0, 9.81);
  RigidBody rigid_body(id, type, size, Transform(pos, angle, scale), mass,
                       is_static, gravity);

  EXPECT_EQ(rigid_body.GetType(), type);
  EXPECT_EQ(rigid_body.GetID(), id);
  EXPECT_FALSE(rigid_body.IsStatic());
  EXPECT_FLOAT_EQ(rigid_body.GetMass(), mass);
  EXPECT_EQ(rigid_body.GetSize(), size);
  EXPECT_EQ(rigid_body.GetLinearVelocity(), zero_vec);
  EXPECT_FLOAT_EQ(rigid_body.GetAngularVelocity(), 0.0);
  EXPECT_FLOAT_EQ(rigid_body.GetStaticFriction(), 0.0);
  EXPECT_FLOAT_EQ(rigid_body.GetDynamicFriction(), 0.0);
  EXPECT_FLOAT_EQ(rigid_body.GetRestitution(), 0.0);
  EXPECT_EQ(rigid_body.GetForce(), zero_vec);
  EXPECT_EQ(rigid_body.GetGravity(), gravity);
  EXPECT_EQ(rigid_body.GetPosition(), pos);
  EXPECT_EQ(rigid_body.GetAngle(), angle);
  EXPECT_EQ(rigid_body.GetScale(), scale);
}

TEST(RigidBodyTest, NoFrictionConstructor) {
  const Vec2f zero_vec = Vec2f(0.0, 0.0);
  const RigidBodyType type = RigidBodyType::Box;
  const std::size_t id = 1;
  const Vec2f size(10.0, 10.0);
  const Vec2f pos(10.0, 10.0);
  const Angle angle = Angle::FromDegrees(45.0);
  const Vec2f scale(2.0, 0.5);
  const float mass = 100.0;
  const bool is_static = false;
  const Vec2f gravity(0.0, 9.81);
  const Vec2f linear_velocity(100.0, 0.0);
  const float angular_velocity = 2.0;
  RigidBody rigid_body(id, type, size, Transform(pos, angle, scale), mass,
                       is_static, gravity, linear_velocity, angular_velocity);
  EXPECT_EQ(rigid_body.GetType(), type);
  EXPECT_EQ(rigid_body.GetID(), id);
  EXPECT_FALSE(rigid_body.IsStatic());
  EXPECT_FLOAT_EQ(rigid_body.GetMass(), mass);
  EXPECT_EQ(rigid_body.GetSize(), size);
  EXPECT_EQ(rigid_body.GetLinearVelocity(), linear_velocity);
  EXPECT_FLOAT_EQ(rigid_body.GetAngularVelocity(), angular_velocity);
  EXPECT_FLOAT_EQ(rigid_body.GetStaticFriction(), 0.0);
  EXPECT_FLOAT_EQ(rigid_body.GetDynamicFriction(), 0.0);
  EXPECT_FLOAT_EQ(rigid_body.GetRestitution(), 0.0);
  EXPECT_EQ(rigid_body.GetForce(), zero_vec);
  EXPECT_EQ(rigid_body.GetGravity(), gravity);
  EXPECT_EQ(rigid_body.GetPosition(), pos);
  EXPECT_EQ(rigid_body.GetAngle(), angle);
  EXPECT_EQ(rigid_body.GetScale(), scale);
}

TEST(RigidBodyTest, BoxSetSize) {
  const Vec2f new_size(10.0, 4.0);
  RigidBody box_rigid_body(1, RigidBodyType::Box, Vec2f(10.0, 10.0), {}, 10.0,
                           false, {});
  box_rigid_body.SetSize(new_size);
  EXPECT_EQ(box_rigid_body.GetSize(), new_size);
}

TEST(RigidBodyTest, CircleSetSize) {
  const Vec2f new_size(10.0, 4.0);
  RigidBody circle_rigid_body(2, RigidBodyType::Circle, Vec2f(10.0, 10.0), {},
                              10.0, false, {});
  circle_rigid_body.SetSize(new_size);
  EXPECT_EQ(circle_rigid_body.GetSize(), Vec2f(new_size.x, new_size.x));
}

TEST(RigidBodyTest, BoxSetScale) {
  const Vec2f new_scale(2.0, 4.0);
  RigidBody box_rigid_body(1, RigidBodyType::Box, Vec2f(10.0, 10.0), {}, 10.0,
                           false, {});
  box_rigid_body.SetScale(new_scale);
  EXPECT_EQ(box_rigid_body.GetScale(), new_scale);
}

TEST(RigidBodyTest, CircleSetScale) {
  const Vec2f new_scale(10.0, 4.0);
  RigidBody circle_rigid_body(2, RigidBodyType::Circle, Vec2f(10.0, 10.0), {},
                              10.0, false, {});
  circle_rigid_body.SetScale(new_scale);
  EXPECT_EQ(circle_rigid_body.GetScale(), Vec2f(new_scale.x, new_scale.x));
}

TEST(RigidBodyTest, BoxInertia) {
  const RigidBody box_rigid_body(2, RigidBodyType::Box, Vec2f(10.0, 10.0), {},
                                 10.0, false, {});
  const float inertia = 166.0;
  EXPECT_FLOAT_EQ(box_rigid_body.GetRotationalInertia(), inertia);
}

TEST(RigidBodyTest, CircleInertia) {
  const RigidBody circle_rigid_body(2, RigidBodyType::Circle,
                                    Vec2f(10.0, 10.0), {}, 10.0, false, {});
  const float inertia = 500;
  EXPECT_FLOAT_EQ(circle_rigid_body.GetRotationalInertia(), inertia);
}

TEST(RigigObjectTest, CircleGetBoundingBox) {
  const RigidBody circle_rigid_body(2, RigidBodyType::Circle,
                                    Vec2f(10.0, 10.0), {}, 10.0, false, {});

  const std::array<Vec2f, 4> expected_vertieces = {
      Vec2f(10.0, -10.0), Vec2f(-10.0, -10.0), Vec2f(-10.0, 10.0),
      Vec2f(10.0, 10.0)};

  const std::array<Vec2f, 4> vertecies = circle_rigid_body.GetBoundingBox();
  for (std::size_t i = 0; i < expected_vertieces.size(); i++) {
    EXPECT_EQ(vertecies[i], expected_vertieces[i]);
  }
}

TEST(RigigObjectTest, BoxGetBoundingBox) {
  const RigidBody box_rigid_body(2, RigidBodyType::Box, Vec2f(10.0, 10.0), {},
                                 10.0, false, {});

  const std::array<Vec2f, 4> expected_vertieces = {
      Vec2f(5.0, -5.0), Vec2f(-5.0, -5.0), Vec2f(-5.0, 5.0), Vec2f(5.0, 5.0)};

  const std::array<Vec2f, 4> vertecies = box_rigid_body.GetBoundingBox();
  for (std::size_t i = 0; i < expected_vertieces.size(); i++) {
    EXPECT_EQ(vertecies[i], expected_vertieces[i]);
  }
}

TEST(RigigObjectTest, RotatedBoxGetBoundingBox) {
  const Transform transform = Transform(Vec2f(), Angle::FromDegrees(45.0));

  const RigidBody box_rigid_body(2, RigidBodyType::Box, Vec2f(10.0, 10.0),
                                 transform, 10.0, false, {});

  const std::array<Vec2f, 4> expected_vertieces = {
      Vec2f(7.0710676908493042, -7.0710676908493042),
      Vec2f(-7.0710676908493042, -7.0710676908493042),
      Vec2f(-7.0710676908493042, 7.0710676908493042),
      Vec2f(7.0710676908493042, 7.0710676908493042)};

  const std::array<Vec2f, 4> vertecies = box_rigid_body.GetBoundingBox();
  for (std::size_t i = 0; i < expected_vertieces.size(); i++) {
    EXPECT_FLOAT_EQ(vertecies[i].x, expected_vertieces[i].x);
    EXPECT_FLOAT_EQ(vertecies[i].y, expected_vertieces[i].y);
  }
}
} // namespace