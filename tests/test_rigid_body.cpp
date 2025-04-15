// local
#include "algo.hpp"
#include "angle.hpp"
#include "collision_body.hpp"
#include "math.hpp"
#include "rigid_body.hpp"
#include "transform.hpp"
#include "vector2.hpp"

//lib
//gtest
#include <gtest/gtest.h>

//std
#include <cstddef>
#include <stdexcept>

namespace {
using namespace pe2d;
TEST(RigidBodyTest, Default_Constructor) {
  RigidBody rigid_body;
  const Vec2f zero_vec = Vec2f(0.0, 0.0);
  EXPECT_EQ(rigid_body.GetType(), 0);
  EXPECT_FALSE(rigid_body.IsStatic());
  EXPECT_FLOAT_EQ(rigid_body.GetMass(), 0.0f);
  EXPECT_FLOAT_EQ(rigid_body.GetRotationalInertia(), 0.0f);
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

TEST(RigidBodyTest, Full_Constructor_NonStatic) {
  const Vec2f zero_vec(0.0, 0.0);
  const BodyType type = BodyType::Box;
  const Vec2f size(10.0, 10.0);
  const Vec2f pos(10.0, 10.0);
  const Angle angle = Angle::FromDegrees(45.0);
  const Vec2f scale(2.0, 0.5);
  const float mass = 100.0;
  const float inertia = algo::CalculateRotationalInertia(type, size, mass);
  const bool is_static = false;
  const Vec2f gravity(0.0f, 9.81f);
  const Vec2f linear_velocity(100.0f, 0.0f);
  const float angular_velocity = 2.0f;
  const float static_friction = 0.5f;
  const float dynamic_friction = 0.37f;
  const float restitution = 0.2f;

  RigidBody rigid_body(type, size, Transform(pos, angle, scale), mass,
                       is_static, gravity, linear_velocity, angular_velocity,
                       static_friction, dynamic_friction, restitution);
  EXPECT_EQ(rigid_body.GetType(), type);
  EXPECT_FALSE(rigid_body.IsStatic());
  EXPECT_FLOAT_EQ(rigid_body.GetMass(), mass);
  EXPECT_FLOAT_EQ(rigid_body.GetRotationalInertia(), inertia);
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

TEST(RigidBodyTest, Full_Constructor_Static) {
  const Vec2f zero_vec(0.0, 0.0);
  const BodyType type = BodyType::Box;
  const Vec2f size(10.0, 10.0);
  const Vec2f pos(10.0, 10.0);
  const Angle angle = Angle::FromDegrees(45.0);
  const Vec2f scale(2.0, 0.5);
  const float mass = 100.0;
  const float inertia = algo::CalculateRotationalInertia(type, size, mass);
  const bool is_static = true;
  const Vec2f gravity(0.0f, 9.81f);
  const Vec2f linear_velocity(100.0f, 0.0f);
  const float angular_velocity = 2.0f;
  const float static_friction = 0.5f;
  const float dynamic_friction = 0.37f;
  const float restitution = 0.2f;

  RigidBody rigid_body(type, size, Transform(pos, angle, scale), mass,
                       is_static, gravity, linear_velocity, angular_velocity,
                       static_friction, dynamic_friction, restitution);
  EXPECT_EQ(rigid_body.GetType(), type);
  EXPECT_TRUE(rigid_body.IsStatic());
  EXPECT_FLOAT_EQ(rigid_body.GetMass(), math::INF);
  EXPECT_FLOAT_EQ(rigid_body.GetRotationalInertia(), math::INF);
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

TEST(RigidBodyTest, NoMotion_NoFriction_Constructor_NonStatic) {
  const Vec2f zero_vec = Vec2f(0.0, 0.0);
  const BodyType type = BodyType::Box;
  const Vec2f size(10.0, 10.0);
  const Vec2f pos(10.0, 10.0);
  const Angle angle = Angle::FromDegrees(45.0);
  const Vec2f scale(2.0, 0.5);
  const float mass = 100.0;
  const float inertia = algo::CalculateRotationalInertia(type, size, mass);
  const bool is_static = false;
  const Vec2f gravity(0.0, 9.81);
  RigidBody rigid_body(type, size, Transform(pos, angle, scale), mass, is_static, gravity);

  EXPECT_EQ(rigid_body.GetType(), type);
  EXPECT_FALSE(rigid_body.IsStatic());
  EXPECT_FLOAT_EQ(rigid_body.GetMass(), mass);
  EXPECT_FLOAT_EQ(rigid_body.GetRotationalInertia(), inertia);
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

TEST(RigidBodyTest, NoMotion_NoFriction_Constructor_Static) {
  const Vec2f zero_vec = Vec2f(0.0, 0.0);
  const BodyType type = BodyType::Box;
  const Vec2f size(10.0, 10.0);
  const Vec2f pos(10.0, 10.0);
  const Angle angle = Angle::FromDegrees(45.0);
  const Vec2f scale(2.0, 0.5);
  const float mass = 100.0;
  const float inertia = algo::CalculateRotationalInertia(type, size, mass);
  const bool is_static = true;
  const Vec2f gravity(0.0, 9.81);
  RigidBody rigid_body(type, size, Transform(pos, angle, scale), mass, is_static, gravity);

  EXPECT_EQ(rigid_body.GetType(), type);
  EXPECT_TRUE(rigid_body.IsStatic());
  EXPECT_FLOAT_EQ(rigid_body.GetMass(), math::INF);
  EXPECT_FLOAT_EQ(rigid_body.GetRotationalInertia(), math::INF);
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

TEST(RigidBodyTest, NoFriction_Constructor_NonStatic) {
  const Vec2f zero_vec = Vec2f(0.0, 0.0);
  const BodyType type = BodyType::Box;
  const Vec2f size(10.0, 10.0);
  const Vec2f pos(10.0, 10.0);
  const Angle angle = Angle::FromDegrees(45.0);
  const Vec2f scale(2.0, 0.5);
  const float mass = 100.0;
  const float inertia = algo::CalculateRotationalInertia(type, size, mass);
  const bool is_static = false;
  const Vec2f gravity(0.0, 9.81);
  const Vec2f linear_velocity(100.0, 0.0);
  const float angular_velocity = 2.0;
  RigidBody rigid_body(type, size, Transform(pos, angle, scale), mass,
                       is_static, gravity, linear_velocity, angular_velocity);
  EXPECT_EQ(rigid_body.GetType(), type);
  EXPECT_FALSE(rigid_body.IsStatic());
  EXPECT_FLOAT_EQ(rigid_body.GetMass(), mass);
  EXPECT_FLOAT_EQ(rigid_body.GetRotationalInertia(), inertia);
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

TEST(RigidBodyTest, NoFriction_Constructor_Static) {
  const Vec2f zero_vec = Vec2f(0.0, 0.0);
  const BodyType type = BodyType::Box;
  const Vec2f size(10.0, 10.0);
  const Vec2f pos(10.0, 10.0);
  const Angle angle = Angle::FromDegrees(45.0);
  const Vec2f scale(2.0, 0.5);
  const float mass = 100.0;
  const float inertia = algo::CalculateRotationalInertia(type, size, mass);
  const bool is_static = true;
  const Vec2f gravity(0.0, 9.81);
  const Vec2f linear_velocity(100.0, 0.0);
  const float angular_velocity = 2.0;
  RigidBody rigid_body(type, size, Transform(pos, angle, scale), mass,
                       is_static, gravity, linear_velocity, angular_velocity);

  EXPECT_EQ(rigid_body.GetType(), type);
  EXPECT_TRUE(rigid_body.IsStatic());
  EXPECT_FLOAT_EQ(rigid_body.GetMass(), math::INF);
  EXPECT_FLOAT_EQ(rigid_body.GetRotationalInertia(), math::INF);
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

TEST(RigidBodyTest, SetSize_Box_ValidSize) {
  const Vec2f size (10.0f, 10.0f);
  RigidBody a(BodyType::Box, size, {}, 1.0f, false, {});
  const Vec2f new_size (1.0f, 1.0f);
  const float expected_inertia = algo::CalculateRotationalInertia(BodyType::Box, new_size, a.GetMass());
  a.SetSize(new_size);
  EXPECT_EQ(a.GetSize(), new_size);
  EXPECT_FLOAT_EQ(a.GetRotationalInertia(), expected_inertia);
}

TEST(RigidBodyTest, SetSize_Box_InvalidSize) {
  const Vec2f size (10.0f, 10.0f);
  RigidBody a(BodyType::Box, size, {}, 1.0f, false, {});
  const Vec2f new_size (-5.0f, 5.0f);
  EXPECT_THROW(a.SetSize(new_size), std::invalid_argument);
}

TEST(RigidBodyTest, SetSize_Circle_ValidSize) {
  const Vec2f size (10.0f, 10.0f);
  RigidBody a(BodyType::Circle, size, {}, 1.0f, false, {});
  const Vec2f new_size (5.0f, 10.0f);
  a.SetSize(new_size);
  const float expected_inertia = algo::CalculateRotationalInertia(BodyType::Circle, new_size, a.GetMass());
  EXPECT_EQ(a.GetSize(), Vec2f(5.0f, 5.0f));
  EXPECT_FLOAT_EQ(a.GetRotationalInertia(), expected_inertia);
}

TEST(RigidBodyTest, SetSize_Circle_InvalidSize) {
  const Vec2f size (10.0f, 20.0f);
  RigidBody a(BodyType::Circle, size, {}, 1.0f, false, {});
  const Vec2f new_size (-5.0f, 5.0f);
  EXPECT_THROW(a.SetSize(new_size), std::invalid_argument);
}

TEST(RigidBodyTest, SetScale_Box_ValidScale) {
  const Vec2f new_scale(2.0f, 4.0f);
  RigidBody box_rigid_body (BodyType::Box, Vec2f(10.0, 10.0), {}, 10.0f,
                           false, {});
  const float rotational_inertia = box_rigid_body.GetRotationalInertia();
  box_rigid_body.SetScale(new_scale);
  EXPECT_EQ(box_rigid_body.GetScale(), new_scale);
  EXPECT_TRUE(rotational_inertia == box_rigid_body.GetRotationalInertia());
}

TEST(RigidBodyTest, SetScale_Box_InvalidScale) {
  const Vec2f new_scale(-2.0f, 4.0f);
  RigidBody a (BodyType::Box, Vec2f(10.0, 10.0), {}, 10.0f,
                           false, {});
  EXPECT_THROW(a.SetScale(new_scale), std::invalid_argument);
}

TEST(RigidBodyTest, SetScale_Circle_ValidScale) {
  const Vec2f new_scale(2.0f, 4.0f);
  RigidBody a(BodyType::Circle, Vec2f(10.0, 10.0), {}, 10.0f,
                           false, {});
  const float rotational_inertia = a.GetRotationalInertia();
  a.SetScale(new_scale);
  EXPECT_EQ(a.GetScale(), Vec2f(new_scale.x, new_scale.x));
  EXPECT_TRUE(rotational_inertia == a.GetRotationalInertia());
}

TEST(RigidBodyTest, SetScale_Circle_InvalidScale) {
  const Vec2f new_scale(-2.0f, 4.0f);
  RigidBody a (BodyType::Circle, Vec2f(10.0, 10.0), {}, 10.0f,
                           false, {});
  EXPECT_THROW(a.SetScale(new_scale), std::invalid_argument);
}

TEST(RigidBodyTest, SetLinearVelocity) {
  RigidBody a(BodyType::Circle, {10, 10}, {}, 10, false, {});
  const Vec2f velocity(100.0f, 0.0f);
  a.SetLinearVelocity(velocity);
  EXPECT_EQ(velocity, a.GetLinearVelocity());
}

TEST(RigidBodyTest, AddLinearVelocity) {
  const Vec2f velocity(100.0f, 0.0f);
  RigidBody a(BodyType::Circle, {10, 10}, {}, 10, false, {});
  a.AddLinearVelocity(velocity);
  EXPECT_EQ(velocity, a.GetLinearVelocity());
}

TEST(RigidBodyTest, SetForce) {
    RigidBody a(BodyType::Circle, {10.0f, 10.0f}, {}, 10.0f, false, {});
    const Vec2f force(100.0f, 0.0f);
    a.SetForce(force);
    EXPECT_EQ(force, a.GetForce());
  }

  TEST(RigidBodyTest, AddForce) {
    RigidBody a(BodyType::Circle, {10.0f, 10.0f}, {}, 10.0f, false, {});
    const Vec2f force(100.0f, 0.0f);
    a.SetForce(force);
    const Vec2f force_offset(69.0f, 0.0f);;
    a.AddForce(force_offset);
    EXPECT_EQ(force + force_offset, a.GetForce());
  }

  TEST(RigidBodyTest, SetGravity) {
    RigidBody a(BodyType::Circle, {10.0f, 10.0f}, {}, 10.0f, false, {});
    const Vec2f gravity(0.0f, 98.1f);
    a.SetGravity(gravity);
    EXPECT_EQ(gravity, a.GetGravity());
  }

TEST(RigidBodyTest, SetStaticFriction_Invalid) {
  RigidBody a(BodyType::Box, {10.0f, 10.0f}, {}, 10.0f, false, {});
  EXPECT_THROW(a.SetStaticFriction(1.1f), std::invalid_argument);
  EXPECT_THROW(a.SetStaticFriction(-0.01f), std::invalid_argument);
}

TEST(RigidBodyTest, SetDynamicFriction_Invalid) {
  RigidBody a(BodyType::Box, {10.0f, 10.0f}, {}, 10.0f, false, {});
  EXPECT_THROW(a.SetDynamicFriction(1.1f), std::invalid_argument);
  EXPECT_THROW(a.SetDynamicFriction(-0.01f), std::invalid_argument);
}

TEST(RigidBodyTest, SetRestitution_Invalid) {
  RigidBody a(BodyType::Box, {10.0f, 10.0f}, {}, 10.0f, false, {});
  EXPECT_THROW(a.SetRestitution(1.1f), std::invalid_argument);
  EXPECT_THROW(a.SetRestitution(-0.01f), std::invalid_argument);
}

TEST(RigidBodyTest, GetBoundingBox_RotatedBox) {
  const Transform transform = Transform(Vec2f(), Angle::FromDegrees(45.0));

  const RigidBody a (BodyType::Box, Vec2f(10.0, 10.0),
                                 transform, 10.0, false, {});

  const std::array<Vec2f, 4> expected_vertieces = {
      Vec2f(7.0710676908493042, -7.0710676908493042),
      Vec2f(-7.0710676908493042, -7.0710676908493042),
      Vec2f(-7.0710676908493042, 7.0710676908493042),
      Vec2f(7.0710676908493042, 7.0710676908493042)};

  const std::array<Vec2f, 4> vertecies = a.GetBoundingBox();
  for (std::size_t i = 0; i < expected_vertieces.size(); i++) {
    EXPECT_FLOAT_EQ(vertecies[i].x, expected_vertieces[i].x);
    EXPECT_FLOAT_EQ(vertecies[i].y, expected_vertieces[i].y);
  }
}

} // namespace