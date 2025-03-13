#include "rigid_body.hpp"
#include "solver.hpp"
#include "vector2.hpp"
#include <gtest/gtest.h>

namespace {
pe2d::RigidBody CreateObject(pe2d::RigidBodyType type, pe2d::Size2d size,
                             pe2d::Pos2d position, bool isStatic) {
  return {0, type, size, pe2d::Transform(position), 10.0, false, {}};
}
class SolverTest : public testing::Test {
protected:
  void AddCollision(float depth, pe2d::Vec2d normal, pe2d::RigidBody &object1,
                    pe2d::RigidBody &object2) {
    pe2d::CollisionPoints points =
        pe2d::CollisionPoints(normal, depth, pe2d::Pos2d(0.0f, 0.0f), true);
    m_Collisions.emplace_back(pe2d::Collision(object1, object2, points));
  }
  void PositionSolverTest(const std::vector<pe2d::Pos2d> &expected_positions) {
    ASSERT_EQ(m_Collisions.size() * 2, expected_positions.size());
    PositionSolver(m_Collisions, 0.0f);
    for (int i = 0; i < m_Collisions.size(); i++) {
      const pe2d::Pos2d position1 = m_Collisions[i].GetObjectA().GetPosition();
      const pe2d::Pos2d position2 = m_Collisions[i].GetObjectB().GetPosition();
      EXPECT_EQ(expected_positions[i].x, position1.x);
      EXPECT_EQ(expected_positions[i].y, position1.y);
      EXPECT_EQ(expected_positions[i + 1].x, position2.x);
      EXPECT_EQ(expected_positions[i + 1].y, position2.y);
    }
  }

protected:
  std::vector<pe2d::Collision> m_Collisions;
};
TEST_F(SolverTest, positionSolverNonStatic) {
  pe2d::RigidBody object1 = CreateObject(pe2d::Circle, pe2d::Size2d(40.0, 40.0),
                                         pe2d::Vec2d(200.0, 200.0), false);
  pe2d::RigidBody object2 = CreateObject(pe2d::Box, pe2d::Size2d(40.0, 40.0),
                                         pe2d::Vec2d(100.0, 100.0), false);
  this->AddCollision(2.0f, pe2d::Vector2(1.0, 0.0), object1, object2);
  const std::vector<pe2d::Pos2d> expected_positions = {
      pe2d::Pos2d(201.0f, 200.0f), pe2d::Pos2d(99.0f, 100.0f)};
  this->PositionSolverTest(expected_positions);
}
TEST_F(SolverTest, positionSolverStatic) {
  pe2d::RigidBody object1 = CreateObject(pe2d::Circle, pe2d::Size2d(40.0, 40.0),
                                         pe2d::Vec2d(200.0, 200.0), true);
  pe2d::RigidBody object2 = CreateObject(pe2d::Box, pe2d::Size2d(40.0, 40.0),
                                         pe2d::Vec2d(100.0, 100.0), false);
  this->AddCollision(2.0f, pe2d::Vec2d(1.0, 0.0), object1, object2);
  const std::vector<pe2d::Pos2d> expected_positions = {
      pe2d::Vec2d(200.0, 200.0), pe2d::Vector2(98.0, 100.0)};
  this->PositionSolverTest(expected_positions);
}
} // namespace