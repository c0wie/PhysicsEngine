#include "algo.hpp"
#include "angle.hpp"
#include "math.hpp"
#include "vector2.hpp"
#include <array>
#include <gtest/gtest.h>

namespace {
using ::testing::Types;
using namespace pe2d;

constexpr double DOUBLE_INACCURACY = 1e-10;
// test_algo_find_contact_points.cpp
TEST(FindCircleBoxContactPoint, OneContactPoint) {
  const std::array<Pos2d, 4> boxVertices = {
      Vec2d(450.0, 450.0), Vec2d(550.0, 450.0), Vec2d(550.0, 550.0),
      Vec2d(450.0, 550.0)};
  const Pos2d circleCenter = Pos2d(575.0, 575.0);
  const Pos2d contactPoint =
      algo::FindCircleBoxContactPoint(boxVertices, circleCenter);
  const Pos2d expectedContactPoint = Pos2d(550.0, 550.0);
  EXPECT_NEAR(contactPoint.x, expectedContactPoint.x, DOUBLE_INACCURACY);
  EXPECT_NEAR(contactPoint.y, expectedContactPoint.y, DOUBLE_INACCURACY);
}

class FindBoxBoxContactPointTest : public testing::Test {
protected:
  void SetUp() override {
    box_vertices = {Pos2d(100.0, 750.0), Pos2d(900.0, 750.0),
                    Pos2d(900.0, 850.0), pe2d::Pos2d(100.0, 850.0)};
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

TEST_F(FindBoxBoxContactPointTest, OneContactPoint) {
  std::array<pe2d::Pos2d, 4> vertices = {
      pe2d::Pos2d(733.425659, 624.799377), pe2d::Pos2d(733.921265, 730.293945),
      pe2d::Pos2d(722.574341, 750.004211), pe2d::Pos2d(682.078735, 682.078735)};
  this->CheckContactPoints(vertices, pe2d::Pos2d(722.574341, 750),
                           pe2d::Pos2d(-1.0, -1.0));
}

TEST_F(FindBoxBoxContactPointTest, TwoContactPoints) {
  std::array<pe2d::Pos2d, 4> vertices = {
      pe2d::Pos2d(487.5, 673.0), pe2d::Pos2d(634.5, 673.0),
      pe2d::Pos2d(634.5, 750.0), pe2d::Pos2d(487.5, 750.0)};
  std::pair<pe2d::Pos2d, pe2d::Pos2d> expected_points =
      std::make_pair(pe2d::Pos2d(634.5, 750.0), pe2d::Pos2d(487.5, 750.0));
  this->CheckContactPoints(vertices, pe2d::Pos2d(634.5, 750.0),
                           pe2d::Pos2d(487.5, 750.0));
}

TEST(PointSegmentDistanceTest, PointOutsideEdge) {
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

TEST(PointSegmentDistanceTest, PointOnEdge) {
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

class RotateVerticesTest : public testing::Test {
protected:
  void SetUp() override {
    m_Center = Pos2d(500.0, 500.0);
    m_TestVertices = {Pos2d(450.0, 450.0), Pos2d(550.0, 450.0),
                      Pos2d(550.0, 550.0), Pos2d(450.0, 550.0)};
  }
  void CheckRotation(const std::array<Pos2d, 4> &expected_vertices,
                     Angle angle) {
    std::array<Pos2d, 4> rotated_vertices = m_TestVertices;
    algo::RotateVertices(rotated_vertices, m_Center, angle);

    EXPECT_FLOAT_EQ(expected_vertices[0].x, rotated_vertices[0].x);
    EXPECT_FLOAT_EQ(expected_vertices[0].y, rotated_vertices[0].y);

    EXPECT_FLOAT_EQ(expected_vertices[1].x, rotated_vertices[1].x);
    EXPECT_FLOAT_EQ(expected_vertices[1].y, rotated_vertices[1].y);

    EXPECT_FLOAT_EQ(expected_vertices[2].x, rotated_vertices[2].x);
    EXPECT_FLOAT_EQ(expected_vertices[2].y, rotated_vertices[2].y);

    EXPECT_FLOAT_EQ(expected_vertices[3].x, rotated_vertices[3].x);
    EXPECT_FLOAT_EQ(expected_vertices[3].y, rotated_vertices[3].y);
  }

protected:
  std::array<Pos2d, 4> m_TestVertices;
  Pos2d m_Center;
};

TEST_F(RotateVerticesTest, PositiveAngle) {
  const Angle angle_45 = Angle::FromDegrees(45.0);
  const std::array<pe2d::Pos2d, 4> expected_vertices_45 = {
      Pos2d(500.0, 429.28932), Pos2d(570.71067, 500.0), Pos2d(500.0, 570.71067),
      Pos2d(429.28932, 500.0)};
  this->CheckRotation(expected_vertices_45, angle_45);

  const Angle angle_90 = Angle::FromDegrees(90.0);
  const std::array<Pos2d, 4> expected_vertices_90 = {
      Vector2(550.0, 450.0), Vector2(550.0, 550.0), Vector2(450.0, 550.0),
      Vector2(450.0, 450.0)};
  this->CheckRotation(expected_vertices_90, angle_90);

  const Angle angle_189 = Angle::FromDegrees(189.0);
  const std::array<Pos2d, 4> expected_vertices_189 = {
      Pos2d(541.56270, 557.20613), Pos2d(442.79386, 541.5627),
      Pos2d(458.43730, 442.79386), Pos2d(557.20613, 458.4373)};
  this->CheckRotation(expected_vertices_189, angle_189);
}

TEST_F(RotateVerticesTest, NegativeAngle) {
  const Angle angle_neg_45 = Angle::FromDegrees(-45.0);
  const std::array<pe2d::Pos2d, 4> expected_vertices_neg_45 = {
      Pos2d(429.28932, 500.0), Pos2d(500.0, 429.28932), Pos2d(570.71067, 500.0),
      Pos2d(500, 570.71069)};
  this->CheckRotation(expected_vertices_neg_45, angle_neg_45);

  const Angle angle_neg_90 = Angle::FromDegrees(-90.0);
  const std::array<Pos2d, 4> expected_vertices_neg_90 = {
      Vector2(450.0, 550.0), Vector2(450.0, 450.0), Vector2(550.0, 450.0),
      Vector2(550.0, 550.0)};
  this->CheckRotation(expected_vertices_neg_90, angle_neg_90);

  const Angle angle_neg_189 = Angle::FromDegrees(-189.0);
  const std::array<Pos2d, 4> expected_vertices_neg_189 = {
      Pos2d(557.20612, 541.56268), Pos2d(458.43732, 557.20612),
      Pos2d(442.79385, 458.43732), Pos2d(541.56268, 442.79385)};
  this->CheckRotation(expected_vertices_neg_189, angle_neg_189);
}

TEST(GetBoxVerticesTest, NonRotatedNonScaled) {
  const Size2d box_size = Size2d(100.0, 100.0);
  const Transform box_transform(Vector2(0.0, 0.0));
  std::array<Pos2d, 4> expected_vertices = {
      Vector2(50.0, -50.0), Vector2(-50.0, -50.0), Vector2(-50.0, 50.0),
      Vector2(50.0, 50.0)};

  EXPECT_EQ(algo::GetBoxVertices(box_size, box_transform), expected_vertices);
}

TEST(GetBoxVerticesTest, RotatedScaled) {
  const Vector2 box_size = Vector2(100.0, 100.0);
  const Transform box_transform = Transform(
      Vector2(500.0, 500.0), Angle::FromDegrees(45.0), Vector2(1.0, 2.0));
  const std::array<Pos2d, 4> expected_box_vertices = {
      Pos2d(606.06604, 464.644653), Pos2d(535.35535, 393.934),
      Pos2d(393.934, 535.35535), Pos2d(464.64465, 606.06604)};
  const std::array<Pos2d, 4> box_vertices =
      algo::GetBoxVertices(box_size, box_transform);

  EXPECT_FLOAT_EQ(box_vertices[0].x, expected_box_vertices[0].x);
  EXPECT_FLOAT_EQ(box_vertices[0].y, expected_box_vertices[0].y);

  EXPECT_FLOAT_EQ(box_vertices[1].x, expected_box_vertices[1].x);
  EXPECT_FLOAT_EQ(box_vertices[1].y, expected_box_vertices[1].y);

  EXPECT_FLOAT_EQ(box_vertices[2].x, expected_box_vertices[2].x);
  EXPECT_FLOAT_EQ(box_vertices[2].y, expected_box_vertices[2].y);

  EXPECT_FLOAT_EQ(box_vertices[3].x, expected_box_vertices[3].x);
  EXPECT_FLOAT_EQ(box_vertices[3].y, expected_box_vertices[3].y);
}

TEST(OverlapTest, Overlap) {
  constexpr double error = 0.0001;
  const Vec2d proj1 = Vec2d(1.0, 3.0);
  const Vec2d proj2 = Vec2d(3.0, 7.0);
  double overlap1 = 0.0;
  EXPECT_EQ(algo::Overlap(proj1, proj2, overlap1), true);
  EXPECT_NEAR(overlap1, 0.0, error);

  const Vec2d proj3 = Vec2d(1.12345, 3.2138);
  const Vec2d proj4 = Vec2d(3.2137, 7.42);
  double overlap2 = 0.0;
  EXPECT_EQ(algo::Overlap(proj3, proj4, overlap2), true);
  EXPECT_NEAR(overlap2, 0.0001f, error);
}

TEST(OverlapTest, NoOverlap) {
  const Vec2d proj1 = Vec2d(1.0, 3.0);
  const Vec2d proj2 = Vec2d(3.1, 7.0);
  double overlap1 = 0.0;
  EXPECT_EQ(algo::Overlap(proj1, proj2, overlap1), false);
  EXPECT_EQ(overlap1, -1.0);

  const Vec2d proj3 = Vec2d(1100.0, 3000.0);
  const Vec2d proj4 = Vec2d(-700.1, -300.0);
  double overlap2 = 0.0;
  EXPECT_EQ(algo::Overlap(proj3, proj4, overlap2), false);
  EXPECT_EQ(overlap2, -1.0);
}

TEST(GetBoxAxesTest, NonRotatedNonScaled) {
  const std::array<Pos2d, 4> vertices = {
      Pos2d(450.0, 450.0), Pos2d(550.0, 450.0), Pos2d(550.0, 550.0),
      Pos2d(450.0, 550.0)};
  const std::array<Vec2d, 2> axes = {Vec2d(0.0, -1.0), Vec2d(1.0, 0.0)};
  EXPECT_EQ(algo::GetBoxAxes(vertices), axes);
}

TEST(GetCircleAxisTest, NonRotatedBoxVertices) {
  const std::array<Pos2d, 4> box_vertices = {
      Pos2d(450.0, 450.0), Pos2d(550.0, 450.0), Pos2d(550.0, 550.0),
      Pos2d(450.0, 550.0)};

  const Pos2d circle_center = Pos2d(600.0, 300.0);
  const Vec2d expected_axis = Vec2d(0.31622776, -0.94868332);
  const Vec2d axis = algo::GetCircleAxis(box_vertices, circle_center);
  EXPECT_FLOAT_EQ(axis.x, expected_axis.x);
  EXPECT_FLOAT_EQ(axis.y, expected_axis.y);
}

TEST(GetCircleAxisTest, RotatedBoxVertices) {
  const std::array<Pos2d, 4> box_vertices = {
      Pos2d(535.355286, 393.93396), Pos2d(606.065979, 464.644592),
      Pos2d(464.644714, 606.06604), Pos2d(393.934021, 535.355408)};

  const Pos2d circle_center = Pos2d(600.0, 300.0);
  const Vec2d expected_axis = Vec2d(0.56691653, -0.823775);
  const Vec2d axis = algo::GetCircleAxis(box_vertices, circle_center);
  EXPECT_FLOAT_EQ(axis.x, expected_axis.x);
  EXPECT_FLOAT_EQ(axis.y, expected_axis.y);
}

TEST(ProjectBoxTest, AlignedAxis) {
  const std::array<Pos2d, 4> box_vertices = {
      Pos2d(339.5, 170.0), Pos2d(418.5, 170.0), Pos2d(418.5, 252.0),
      Pos2d(339.5, 252.0)};

  const Vec2d axis1 = Vec2d(1.0, 0.0);
  const Vec2d expected_projection1 = Vec2d(339.5, 418.5);
  const Vector2 projection1 = algo::Project(box_vertices, axis1);
  EXPECT_FLOAT_EQ(projection1.x, expected_projection1.x);
  EXPECT_FLOAT_EQ(projection1.y, expected_projection1.y);

  const Vector2 axis2 = Vector2(0.0, 1.0);
  const Vector2 expected_projection2 = Vector2(170.0, 252.0);
  const Vector2 projection2 = algo::Project(box_vertices, axis2);
  EXPECT_FLOAT_EQ(projection2.x, expected_projection2.x);
  EXPECT_FLOAT_EQ(projection2.y, expected_projection2.y);
}

TEST(ProjectBoxTest, NonAlignedAxis) {
  const std::array<Pos2d, 4> box_vertices = {
      Pos2d(339.5, 170.0), Pos2d(418.5, 170.0), Pos2d(418.5, 252.0),
      Pos2d(339.5, 252.0)};

  const Vec2d axis1 = Vec2d(0.05, 0.95);
  const Vec2d expected_projection1 = Vec2d(178.475006, 260.324982);
  const Vector2 projection1 = algo::Project(box_vertices, axis1);
  EXPECT_FLOAT_EQ(projection1.x, expected_projection1.x);
  EXPECT_FLOAT_EQ(projection1.y, expected_projection1.y);

  const Vec2d axis2 = Vec2d(0.21, 0.79);
  const Vec2d expected_projection2 = Vec2d(205.595001, 286.964996);
  const Vector2 projection2 = algo::Project(box_vertices, axis2);
  EXPECT_FLOAT_EQ(projection2.x, expected_projection2.x);
  EXPECT_FLOAT_EQ(projection2.y, expected_projection2.y);
}

TEST(ProjectCircleTest, AlignedAxis) {
  const Pos2d circle_center = Pos2d(70.0, 800.0);
  const double radius = 60.0;
  const Vec2d axis1 = Vec2d(0.0, 1.0);
  const Vec2d expected_projection1 = Vec2d(740.0, 860.0);
  const Vec2d projection1 = algo::ProjectCircle(circle_center, radius, axis1);

  EXPECT_FLOAT_EQ(projection1.x, expected_projection1.x);
  EXPECT_FLOAT_EQ(projection1.y, expected_projection1.y);

  const Vec2d axis2 = Vec2d(1.0, 0.0);
  const Vec2d expected_projection2 = Vec2d(10.0, 130.0);
  const Vec2d projection2 = algo::ProjectCircle(circle_center, radius, axis2);
  EXPECT_FLOAT_EQ(projection2.x, expected_projection2.x);
  EXPECT_FLOAT_EQ(projection2.y, expected_projection2.y);
}

TEST(ProjectCircleTest, NonAlignedAxis) {
  const Pos2d circle_center = Pos2d(500.0, 480.0);
  const double radius = 78.0;
  const Vec2d axis1 = Vec2d(0.5, 0.5);
  const Vec2d expected_projection1 = Vec2d(451.0, 529.0);
  const Vec2d projection1 = algo::ProjectCircle(circle_center, radius, axis1);
  EXPECT_FLOAT_EQ(projection1.x, expected_projection1.x);
  EXPECT_FLOAT_EQ(projection1.y, expected_projection1.y);

  const Vec2d axis2 = Vec2d(0.7, 0.3);
  const Vec2d expected_projection2 = Vec2d(448.76001, 539.23999);
  const Vector2 projection2 = algo::ProjectCircle(circle_center, radius, axis2);

  EXPECT_FLOAT_EQ(projection2.x, expected_projection2.x);
  EXPECT_FLOAT_EQ(projection2.y, expected_projection2.y);
}
} // namespace