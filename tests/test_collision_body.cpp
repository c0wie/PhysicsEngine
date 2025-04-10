// local
#include "collision_body.hpp"
#include "vector2.hpp"

//lib
//gtest
#include <gtest/gtest.h>

namespace {
  using namespace pe2d;
  TEST(CollisionBodyTest, Default_Constuctor) {
    CollisionBody a;
    EXPECT_FALSE(a.IsStatic());
    EXPECT_EQ(a.GetSize(), Vec2f(0.0f, 0.0f));
    EXPECT_EQ(a.GetType(), 0);
    EXPECT_EQ(a.GetLinearVelocity(), Vec2f(0.0f, 0.0f));
    EXPECT_EQ(a.GetPosition(), Vec2f(0.0f, 0.0f));
  }

  TEST(CollisionBodyTest, NoVelocity_Constructor) {
    const float is_static = false;
    const BodyType body_type = BodyType::Box;
    const Vec2f size(10.0f, 10.0f);
    const Vec2f position(100.0f, 100.0f);
    CollisionBody a(body_type, size, position, is_static);
    EXPECT_EQ(a.IsStatic(), is_static);
    EXPECT_EQ(a.GetSize(), size);
    EXPECT_EQ(a.GetType(), body_type);
    EXPECT_EQ(a.GetLinearVelocity(), Vec2f(0.0f, 0.0f));
    EXPECT_EQ(a.GetPosition(), position);
  }

  TEST(CollisionBodyTest, Full_Constructor) {
    const float is_static = false;
    const BodyType body_type = BodyType::Box;
    const Vec2f size(10.0f, 10.0f);
    const Vec2f position(100.0f, 100.0f);
    const Vec2f velocity(100.0f, 0.0f);
    CollisionBody a(body_type, size, position, velocity, is_static);
    EXPECT_EQ(a.IsStatic(), is_static);
    EXPECT_EQ(a.GetSize(), size);
    EXPECT_EQ(a.GetType(), body_type);
    EXPECT_EQ(a.GetLinearVelocity(), velocity);
    EXPECT_EQ(a.GetPosition(), position);
  }

  TEST(CollisionBodyTest, SetSize_Box_ValidSize) {
    const Vec2f size (10.0f, 10.0f);
    CollisionBody a(BodyType::Box, size, Vec2f(), Vec2f(), false);
    const Vec2f new_size (5.0f, 5.0f);
    a.SetSize(new_size);
    EXPECT_EQ(a.GetSize(), new_size);
  }

  TEST(CollisionBodyTest, SetSize_Box_InvalidSize) {
    const Vec2f size (10.0f, 10.0f);
    CollisionBody a(BodyType::Box, size, Vec2f(), Vec2f(), false);
    const Vec2f new_size (-5.0f, 5.0f);
    EXPECT_THROW(a.SetSize(new_size), std::invalid_argument);
  }

  TEST(CollisionBodyTest, SetSize_Circle_ValidSize) {
    const Vec2f size (10.0f, 10.0f);
    CollisionBody a(BodyType::Circle, size, Vec2f(), Vec2f(), false);
    const Vec2f new_size (5.0f, 10.0f);
    a.SetSize(new_size);
    EXPECT_EQ(a.GetSize(), Vec2f(5.0f, 5.0f));
  }

  TEST(CollisionBodyTest, SetSize_Circle_InvalidSize) {
    const Vec2f size (10.0f, 20.0f);
    CollisionBody a(BodyType::Circle, size, Vec2f(), Vec2f(), false);
    const Vec2f new_size (-5.0f, 5.0f);
    EXPECT_THROW(a.SetSize(new_size), std::invalid_argument);
  }

  TEST(CollisionBodyTest, SetPosition) {
    const Vec2f position(10.0f, 20.0f);
    CollisionBody a(BodyType::Circle, {10.0f, 10.0f}, Vec2f(), Vec2f(), false);
    a.SetPosition(position);
    EXPECT_EQ(position, a.GetPosition());
  }

  TEST(CollisionBodyTest, Move) {
    const Vec2f position(10.0f, 20.0f);
    CollisionBody a(BodyType::Circle, {10.0f, 10.0f}, position, false);
    const Vec2f possition_offset(5.0f, -5.0f);
    a.Move(possition_offset);
    EXPECT_EQ(position + possition_offset, a.GetPosition());
  }

  TEST(CollisionBodyTest, SetLinearVelocity) {
    CollisionBody a(BodyType::Circle, {10.0f, 10.0f}, Vec2f(), false);
    const Vec2f velocity(100.0f, 0.0f);
    a.SetLinearVelocity(velocity);
    EXPECT_EQ(velocity, a.GetLinearVelocity());
  }

  TEST(CollisionBodyTest, AddLinearVelocity) {
    const Vec2f velocity(100.0f, 0.0f);
    CollisionBody a(BodyType::Circle, {10.0f, 10.0f}, Vec2f(), velocity, false);
    a.AddLinearVelocity(velocity);
    EXPECT_EQ(velocity + velocity, a.GetLinearVelocity());
  }

  TEST(CollisionBodyTest, GetBoundingBox_Box) {
    const CollisionBody a(BodyType::Box, {10.0f, 10.0f}, {}, {}, false);
    const std::array<Vec2f, 4> expected_vertieces = {
      Vec2f(5.0, -5.0), 
      Vec2f(-5.0, -5.0),
      Vec2f(-5.0, 5.0),
      Vec2f(5.0, 5.0)
    };
    const std::array<Vec2f, 4> vertecies = a.GetBoundingBox();
    for (std::size_t i = 0; i < expected_vertieces.size(); i++) {
      EXPECT_EQ(vertecies[i], expected_vertieces[i]);
    }
  }

  TEST(RigigObjectTest, GetBoundingBox_Circle) {
    const CollisionBody a(BodyType::Circle, {10.0f, 10.0f}, {}, {}, false);
  
    const std::array<Vec2f, 4> expected_vertieces = {
        Vec2f(10.0, -10.0), Vec2f(-10.0, -10.0), Vec2f(-10.0, 10.0),
        Vec2f(10.0, 10.0)};
  
    const std::array<Vec2f, 4> vertecies = a.GetBoundingBox();
    for (std::size_t i = 0; i < expected_vertieces.size(); i++) {
      EXPECT_EQ(vertecies[i], expected_vertieces[i]);
    }
  }
}