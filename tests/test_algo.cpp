#include "algo.hpp"
#include "math.hpp"
#include "vector2.hpp"
#include <gtest/gtest.h>

namespace {
using ::testing::Types;

constexpr double DOUBLE_INACCURACY = 1e-12;
// test_algo_find_contact_points.cpp
TEST(FindCircleBoxContactPoint, OneContactPoint) {
  const std::array<pe2d::Pos2d, 4> boxVertices = {
      pe2d::Vec2d(450.0, 450.0), pe2d::Vec2d(550.0, 450.0),
      pe2d::Vec2d(550.0, 550.0), pe2d::Vec2d(450.0, 550.0)};
  const pe2d::Pos2d circleCenter = pe2d::Pos2d(575.0, 575.0);
  const pe2d::Pos2d contactPoint =
      pe2d::algo::FindCircleBoxContactPoint(boxVertices, circleCenter);
  const pe2d::Pos2d expectedContactPoint = pe2d::Pos2d(550.0, 550.0);
  EXPECT_NEAR(contactPoint.x, expectedContactPoint.x, DOUBLE_INACCURACY);
  EXPECT_NEAR(contactPoint.y, expectedContactPoint.y, DOUBLE_INACCURACY);
}

class FindBoxBoxContactPoint : public testing::Test {
protected:
  void SetUp() override {
    box_vertices = {pe2d::Pos2d(100.0, 750.0), pe2d::Pos2d(900.0, 750.0),
                    pe2d::Pos2d(900.0, 850.0), pe2d::Pos2d(100.0, 850.0)};
  }
  void CheckContactPoints(std::array<pe2d::Pos2d, 4> vertices,
                          pe2d::Pos2d expected_pointA,
                          pe2d::Pos2d expected_pointB) {
    const std::pair<pe2d::Pos2d, pe2d::Pos2d> points =
        pe2d::algo::FindBoxBoxContactPoint(vertices, box_vertices);
    std::cout << points.first.GetString() << " " << points.second.GetString()
              << '\n';
    std::cout << expected_pointA.GetString() << " \n"
              << expected_pointB.GetString() << '\n';
    EXPECT_NEAR(points.first.x, expected_pointA.x, DOUBLE_INACCURACY);
    EXPECT_NEAR(points.first.y, expected_pointA.y, DOUBLE_INACCURACY);
    EXPECT_NEAR(points.second.x, expected_pointB.x, DOUBLE_INACCURACY);
    EXPECT_NEAR(points.second.y, expected_pointB.y, DOUBLE_INACCURACY);
  }

protected:
  std::array<pe2d::Pos2d, 4> box_vertices;
};

TEST_F(FindBoxBoxContactPoint, OneContactPoint) {
  std::array<pe2d::Pos2d, 4> vertices = {
      pe2d::Pos2d(733.425659, 624.799377), pe2d::Pos2d(733.921265, 730.293945),
      pe2d::Pos2d(722.574341, 750.004211), pe2d::Pos2d(682.078735, 682.078735)};
  this->CheckContactPoints(vertices, pe2d::Pos2d(722.574341, 750),
                           pe2d::Pos2d(-1.0, -1.0));
}

TEST_F(FindBoxBoxContactPoint, TwoContactPoints) {
  std::array<pe2d::Pos2d, 4> vertices = {
      pe2d::Pos2d(487.5, 673.0), pe2d::Pos2d(634.5, 673.0),
      pe2d::Pos2d(634.5, 750.0), pe2d::Pos2d(487.5, 750.0)};
  std::pair<pe2d::Pos2d, pe2d::Pos2d> expected_points =
      std::make_pair(pe2d::Pos2d(634.5, 750.0), pe2d::Pos2d(487.5, 750.0));
  this->CheckContactPoints(vertices, pe2d::Pos2d(634.5, 750.0),
                           pe2d::Pos2d(487.5, 750.0));
}

TEST(PointSegmentDistanceTest, pointOutsideEdge) {
  const pe2d::Pos2d point = pe2d::Pos2d(5.0, 5.0);
  const pe2d::Pos2d vertexA = pe2d::Pos2d(0.0, 0.0);
  const pe2d::Pos2d vertexB = pe2d::Pos2d(10.0, 0.0);
  double distance_squared = 0.0;
  pe2d::Pos2d contact_point = pe2d::Pos2d(0.0, 0.0);
  const pe2d::Pos2d expected_contact_point = pe2d::Pos2d(5.0, 0.0);
  const double expectedDistance = 25.0;
  pe2d::algo::PointSegmentDistance(point, vertexA, vertexB, distance_squared,
                                   contact_point);
  EXPECT_EQ(distance_squared, expectedDistance);
  EXPECT_EQ(contact_point.x, expected_contact_point.x);
  EXPECT_EQ(contact_point.y, expected_contact_point.y);
}

TEST(PointSegmentDistanceTest, pointOnEdge) {
  const pe2d::Vector2 point = pe2d::Vector2(5.0, 5.0);
  const pe2d::Vector2 vertexA = pe2d::Vector2(0.0, 5.0);
  const pe2d::Vector2 vertexB = pe2d::Vector2(10.0, 5.0);
  double distanceSquared = 0.0;
  pe2d::Vector2 contactPoint = pe2d::Vector2(0.0, 0.0);
  const pe2d::Vector2 expectedContactPoint = pe2d::Vector2(5.0, 5.0);
  const double expectedDistance = 0.0;
  pe2d::algo::PointSegmentDistance(point, vertexA, vertexB, distanceSquared,
                                   contactPoint);
  EXPECT_EQ(distanceSquared, expectedDistance);
  EXPECT_EQ(contactPoint.x, expectedContactPoint.x);
  EXPECT_EQ(contactPoint.y, expectedContactPoint.y);
}
// end test_algo_find_contact_points.cpp

// test_algo_rotation.cpp
class RotateVertices : public testing::Test {
protected:
  void SetUp() override {
    m_Center = pe2d::Pos2d(500.0, 500.0);
    m_TestVertices = {pe2d::Pos2d(450.0, 450.0), pe2d::Pos2d(550.0, 450.0),
                      pe2d::Pos2d(550.0, 550.0), pe2d::Pos2d(450.0, 550.0)};
  }
  void CheckRotation(const std::array<pe2d::Pos2d, 4> &expected_vertices,
                     double angle_deegres) {
    const double angle_radians = pe2d::math::DeegresToRadians(angle_deegres);
    std::array<pe2d::Pos2d, 4> rotated_vertices = m_TestVertices;
    pe2d::algo::RotateVertices(rotated_vertices, m_Center, angle_radians);
    for (int i = 0; i < expected_vertices.size(); i++) {
      EXPECT_NEAR(rotated_vertices[i].x, expected_vertices[i].x,
                  DOUBLE_INACCURACY);
      EXPECT_NEAR(rotated_vertices[i].y, expected_vertices[i].y,
                  DOUBLE_INACCURACY);
    }
  }

protected:
  std::array<pe2d::Pos2d, 4> m_TestVertices;
  pe2d::Pos2d m_Center;
};

TEST_F(RotateVertices, PositiveAngle) {
  const std::array<pe2d::Pos2d, 4> expected_vertices_45 = {
      pe2d::Pos2d(500.0, 429.289323), pe2d::Pos2d(570.710677, 500.0),
      pe2d::Pos2d(500.0, 570.710677), pe2d::Pos2d(429.289323, 500.0)};
  this->CheckRotation(expected_vertices_45, 45.0);

  const std::array<pe2d::Pos2d, 4> expected_vertices_90 = {
      pe2d::Vector2(550.0, 450.0), pe2d::Vector2(550.0, 550.0),
      pe2d::Vector2(450.0, 550.0), pe2d::Vector2(450.0, 450.0)};
  this->CheckRotation(expected_vertices_90, 90.0);

  const std::array<pe2d::Pos2d, 4> expected_vertices_189 = {
      pe2d::Pos2d(541.562700, 557.206137), pe2d::Pos2d(442.793863, 541.5627),
      pe2d::Pos2d(458.437300, 442.793863), pe2d::Pos2d(557.206137, 458.4373)};
  this->CheckRotation(expected_vertices_189, 189.0);
}
// end test_algo_rotation.cpp

TEST(GetBoxVerticesTest, notRotatednotScaled) {
  const pe2d::Size2d box_size = pe2d::Size2d(100.0, 100.0);
  const pe2d::Transform box_transform = pe2d::Transform(
      pe2d::Vector2(500.0, 500.0), 0.0, pe2d::Vector2(1.0, 1.0));
  std::array<pe2d::Pos2d, 4> expected_vertices = {
      pe2d::Vector2(450.0, 450.0), pe2d::Vector2(550.0, 450.0),
      pe2d::Vector2(550.0, 550.0), pe2d::Vector2(450.0, 550.0)};
  EXPECT_EQ(pe2d::algo::GetBoxVertices(box_size, box_transform),
            expected_vertices);
}

TEST(GetBoxVerticesTest, rotatedScaled) {
  const pe2d::Vector2 box_size = pe2d::Vector2(100.0, 100.0);
  const pe2d::Transform box_transform = pe2d::Transform(
      pe2d::Vector2(500.0, 500.0), pe2d::math::DeegresToRadians(45.0),
      pe2d::Vector2(1.0, 2.0));
  std::array<pe2d::Pos2d, 4> expected = {
      pe2d::Pos2d(535.355347, 393.93399), pe2d::Pos2d(606.06604, 464.644653),
      pe2d::Pos2d(464.644653, 606.06604), pe2d::Pos2d(393.93399, 535.355347)};
  EXPECT_EQ(pe2d::algo::GetBoxVertices(box_size, box_transform), expected);
}

TEST(OverlapTest, isOverlaping) {
  constexpr double error = 0.0001;
  const pe2d::Vec2d proj1 = pe2d::Vec2d(1.0, 3.0);
  const pe2d::Vec2d proj2 = pe2d::Vec2d(3.0, 7.0);
  double overlap1 = 0.0;
  EXPECT_EQ(pe2d::algo::Overlap(proj1, proj2, overlap1), true);
  EXPECT_NEAR(overlap1, 0.0, error);

  const pe2d::Vec2d proj3 = pe2d::Vec2d(1.12345, 3.2138);
  const pe2d::Vec2d proj4 = pe2d::Vec2d(3.2137, 7.42);
  double overlap2 = 0.0;
  EXPECT_EQ(pe2d::algo::Overlap(proj3, proj4, overlap2), true);
  EXPECT_NEAR(overlap2, 0.0001f, error);
}

TEST(OverlapTest, notOverlaping) {
  const pe2d::Vec2d proj1 = pe2d::Vec2d(1.0, 3.0);
  const pe2d::Vec2d proj2 = pe2d::Vec2d(3.1, 7.0);
  double overlap1 = 0.0;
  EXPECT_EQ(pe2d::algo::Overlap(proj1, proj2, overlap1), false);
  EXPECT_EQ(overlap1, -1.0);

  const pe2d::Vec2d proj3 = pe2d::Vec2d(1100.0, 3000.0);
  const pe2d::Vec2d proj4 = pe2d::Vec2d(-700.1, -300.0);
  double overlap2 = 0.0;
  EXPECT_EQ(pe2d::algo::Overlap(proj3, proj4, overlap2), false);
  EXPECT_EQ(overlap2, -1.0);
}

TEST(GetBoxAxesTest, notRotatednotScaled) {
  const std::array<pe2d::Pos2d, 4> vertices = {
      pe2d::Pos2d(450.0, 450.0), pe2d::Pos2d(550.0, 450.0),
      pe2d::Pos2d(550.0, 550.0), pe2d::Pos2d(450.0, 550.0)};
  const std::array<pe2d::Vec2d, 2> axes = {pe2d::Vec2d(0.0, -1.0),
                                           pe2d::Vec2d(1.0, 0.0)};
  EXPECT_EQ(pe2d::algo::GetBoxAxes(vertices), axes);
}

class GetCircleAxisTest : public testing::Test {
protected:
  void SetUp() override {
    m_TestVertices = {pe2d::Pos2d(535.355286, 393.93396),
                      pe2d::Pos2d(606.065979, 464.644592),
                      pe2d::Pos2d(464.644714, 606.06604),
                      pe2d::Pos2d(393.934021, 535.355408)};
  }
  void checkAxis(pe2d::Pos2d circle_center, pe2d::Vec2d expected_axis) {
    const double error = 0.000001f;
    const pe2d::Vec2d result_axis =
        pe2d::algo::GetCircleAxis(this->m_TestVertices, circle_center);
    EXPECT_NEAR(result_axis.x, expected_axis.x, error);
    EXPECT_NEAR(result_axis.y, expected_axis.y, error);
  }

protected:
  std::array<pe2d::Pos2d, 4> m_TestVertices;
};

TEST_F(GetCircleAxisTest, standardScenario) {
  const pe2d::Pos2d circle_center1 = pe2d::Pos2d(600.0, 300.0);
  const pe2d::Vec2d expected_axis1 = pe2d::Vec2d(-0.566917, 0.823775);
  this->checkAxis(circle_center1, expected_axis1);

  const pe2d::Pos2d circle_center2 = pe2d::Pos2d(0.0, 420.0);
  const pe2d::Vec2d expected_axis2 = pe2d::Vec2d(0.959700, 0.281028f);
  this->checkAxis(circle_center2, expected_axis2);
}

class ProjectTest : public testing::Test {
protected:
  void SetUp() override {
    m_TestVertices = {pe2d::Pos2d(339.5, 170.0), pe2d::Pos2d(418.5, 170.0),
                      pe2d::Pos2d(418.5, 252.0), pe2d::Pos2d(339.5, 252.0)};
  }
  void CheckProjection(pe2d::Vec2d expected_projection, pe2d::Vec2d axis) {
    const pe2d::Vector2 projection = pe2d::algo::Project(m_TestVertices, axis);
    EXPECT_EQ(projection.x, expected_projection.x);
    EXPECT_EQ(projection.y, expected_projection.y);
  }

protected:
  std::array<pe2d::Pos2d, 4> m_TestVertices;
};

TEST_F(ProjectTest, alignedAxis) {
  const pe2d::Vec2d axis1 = pe2d::Vec2d(1.0, 0.0);
  const pe2d::Vec2d expected_projection1 = pe2d::Vec2d(339.5, 418.5);
  this->CheckProjection(expected_projection1, axis1);

  const pe2d::Vector2 axis2 = pe2d::Vector2(0.0, 1.0);
  const pe2d::Vector2 expected_projection2 = pe2d::Vector2(170.0, 252.0);
  this->CheckProjection(expected_projection2, axis2);
}

TEST_F(ProjectTest, nonAlignedAxis) {
  const pe2d::Vec2d axis1 = pe2d::Vec2d(0.05, 0.95);
  const pe2d::Vec2d expected_projection1 = pe2d::Vec2d(178.475006, 260.324982);
  this->CheckProjection(expected_projection1, axis1);

  const pe2d::Vec2d axis2 = pe2d::Vec2d(0.21, 0.79);
  const pe2d::Vec2d expected_projection2 = pe2d::Vec2d(205.595001, 286.964996);
  this->CheckProjection(expected_projection2, axis2);
}

TEST(ProjectCircleTest, alignedAxis) {
  const pe2d::Pos2d circle_center = pe2d::Pos2d(70.0, 800.0);
  const double radius = 60.0;
  const pe2d::Vec2d axis1 = pe2d::Vec2d(0.0, 1.0);
  const pe2d::Vec2d expected_projection1 = pe2d::Vec2d(740.0, 860.0);
  const pe2d::Vec2d projection1 =
      pe2d::algo::ProjectCircle(circle_center, radius, axis1);
  EXPECT_EQ(projection1, expected_projection1);

  const pe2d::Vec2d axis2 = pe2d::Vec2d(1.0, 0.0);
  const pe2d::Vec2d expected_projection2 = pe2d::Vec2d(10.0, 130.0);
  const pe2d::Vec2d projection2 =
      pe2d::algo::ProjectCircle(circle_center, radius, axis2);
  EXPECT_EQ(projection2, expected_projection2);
}

TEST(ProjectCircleTest, nonAlignedAxis) {
  const pe2d::Pos2d circle_center = pe2d::Pos2d(500.0, 480.0);
  const double radius = 78.0;
  const pe2d::Vec2d axis1 = pe2d::Vec2d(0.5, 0.5);
  const pe2d::Vec2d expected_projection1 = pe2d::Vec2d(451.0, 529.0);
  const pe2d::Vec2d projection1 =
      pe2d::algo::ProjectCircle(circle_center, radius, axis1);
  EXPECT_EQ(projection1, expected_projection1);

  const pe2d::Vec2d axis2 = pe2d::Vec2d(0.7, 0.3);
  const pe2d::Vec2d expected_projection2 = pe2d::Vec2d(448.76001, 539.23999);
  const pe2d::Vector2 projection2 =
      pe2d::algo::ProjectCircle(circle_center, radius, axis2);
  EXPECT_EQ(projection2, expected_projection2);
}
} // namespace