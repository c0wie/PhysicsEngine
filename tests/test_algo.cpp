#include "algo.hpp"
#include "angle.hpp"
#include "math.hpp"
#include "vector2.hpp"
#include "vector2.hpp"
#include <array>
#include <gtest/gtest.h>

namespace {
using ::testing::Types;
using namespace pe2d;

TEST(FindCircleBoxContactPoint, OneContactPoint) {
  const std::array<Vec2f, 4> boxVertices = {
      Vec2f(450.0, 450.0), Vec2f(550.0, 450.0), Vec2f(550.0, 550.0),
      Vec2f(450.0, 550.0)};
  const Vec2f circleCenter = Vec2f(575.0, 575.0);
  const Vec2f contactPoint =
      algo::FindCircleBoxContactPoint(boxVertices, circleCenter);
  const Vec2f expectedContactPoint = Vec2f(550.0, 550.0);
  EXPECT_FLOAT_EQ(contactPoint.x, expectedContactPoint.x);
  EXPECT_FLOAT_EQ(contactPoint.y, expectedContactPoint.y);
}

class FindBoxBoxContactPointTest : public testing::Test {
protected:
  void SetUp() override {
    box_vertices = {Vec2f(100.0, 750.0), Vec2f(900.0, 750.0),
                    Vec2f(900.0, 850.0), pe2d::Vec2f(100.0, 850.0)};
  }
  void CheckContactPoints(std::array<pe2d::Vec2f, 4> vertices,
                          pe2d::Vec2f expected_pointA,
                          pe2d::Vec2f expected_pointB) {
    const std::pair<pe2d::Vec2f, pe2d::Vec2f> points =
        pe2d::algo::FindBoxBoxContactPoint(vertices, box_vertices);
    std::cout << points.first.GetString() << " " << points.second.GetString()
              << '\n';
    std::cout << expected_pointA.GetString() << " \n"
              << expected_pointB.GetString() << '\n';
    EXPECT_FLOAT_EQ(points.first.x, expected_pointA.x);
    EXPECT_FLOAT_EQ(points.first.y, expected_pointA.y);
    EXPECT_FLOAT_EQ(points.second.x, expected_pointB.x);
    EXPECT_FLOAT_EQ(points.second.y, expected_pointB.y);
  }

protected:
  std::array<pe2d::Vec2f, 4> box_vertices;
};

TEST_F(FindBoxBoxContactPointTest, OneContactPoint) {
  std::array<pe2d::Vec2f, 4> vertices = {
      pe2d::Vec2f(733.425659, 624.799377), pe2d::Vec2f(733.921265, 730.293945),
      pe2d::Vec2f(722.574341, 750.004211), pe2d::Vec2f(682.078735, 682.078735)};
  this->CheckContactPoints(vertices, pe2d::Vec2f(722.574341, 750),
                           pe2d::Vec2f(-1.0, -1.0));
}

TEST_F(FindBoxBoxContactPointTest, TwoContactPoints) {
  std::array<pe2d::Vec2f, 4> vertices = {
      pe2d::Vec2f(487.5, 673.0), pe2d::Vec2f(634.5, 673.0),
      pe2d::Vec2f(634.5, 750.0), pe2d::Vec2f(487.5, 750.0)};
  std::pair<pe2d::Vec2f, pe2d::Vec2f> expected_points =
      std::make_pair(pe2d::Vec2f(634.5, 750.0), pe2d::Vec2f(487.5, 750.0));
  this->CheckContactPoints(vertices, pe2d::Vec2f(634.5, 750.0),
                           pe2d::Vec2f(487.5, 750.0));
}

TEST(PointSegmentDistanceTest, PointOutsideEdge) {
  const pe2d::Vec2f point = pe2d::Vec2f(5.0, 5.0);
  const pe2d::Vec2f vertexA = pe2d::Vec2f(0.0, 0.0);
  const pe2d::Vec2f vertexB = pe2d::Vec2f(10.0, 0.0);
  float distance_squared = 0.0;
  pe2d::Vec2f contact_point = pe2d::Vec2f(0.0, 0.0);
  const pe2d::Vec2f expected_contact_point = pe2d::Vec2f(5.0, 0.0);
  const float expectedDistance = 25.0;
  pe2d::algo::PointSegmentDistance(point, vertexA, vertexB, distance_squared,
                                   contact_point);
  EXPECT_EQ(distance_squared, expectedDistance);
  EXPECT_EQ(contact_point.x, expected_contact_point.x);
  EXPECT_EQ(contact_point.y, expected_contact_point.y);
}

TEST(PointSegmentDistanceTest, PointOnEdge) {
  const pe2d::Vec2f point(5.0f, 5.0f);
  const pe2d::Vec2f vertexA(0.0f, 5.0f);
  const pe2d::Vec2f vertexB(10.0f, 5.0f);
  float distanceSquared = 0.0;
  pe2d::Vec2f contactPoint(0.0, 0.0);
  const pe2d::Vec2f expectedContactPoint(5.0, 5.0);
  const float expectedDistance = 0.0;
  pe2d::algo::PointSegmentDistance(point, vertexA, vertexB, distanceSquared,
                                   contactPoint);
  EXPECT_EQ(distanceSquared, expectedDistance);
  EXPECT_EQ(contactPoint.x, expectedContactPoint.x);
  EXPECT_EQ(contactPoint.y, expectedContactPoint.y);
}

class RotateVerticesTest : public testing::Test {
protected:
  void SetUp() override {
    m_Center = Vec2f(500.0, 500.0);
    m_TestVertices = {Vec2f(450.0, 450.0), Vec2f(550.0, 450.0),
                      Vec2f(550.0, 550.0), Vec2f(450.0, 550.0)};
  }
  void CheckRotation(const std::array<Vec2f, 4> &expected_vertices,
                     Angle angle) {
    std::array<Vec2f, 4> rotated_vertices = m_TestVertices;
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
  std::array<Vec2f, 4> m_TestVertices;
  Vec2f m_Center;
};

TEST_F(RotateVerticesTest, PositiveAngle) {
  const Angle angle_45 = Angle::FromDegrees(45.0);
  const std::array<pe2d::Vec2f, 4> expected_vertices_45 = {
      Vec2f(500.0, 429.28932), Vec2f(570.71067, 500.0), Vec2f(500.0, 570.71067),
      Vec2f(429.28932, 500.0)};
  this->CheckRotation(expected_vertices_45, angle_45);

  const Angle angle_90 = Angle::FromDegrees(90.0);
  const std::array<Vec2f, 4> expected_vertices_90 = {
      Vec2f(550.0, 450.0), Vec2f(550.0, 550.0), Vec2f(450.0, 550.0),
      Vec2f(450.0, 450.0)};
  this->CheckRotation(expected_vertices_90, angle_90);

  const Angle angle_189 = Angle::FromDegrees(189.0);
  const std::array<Vec2f, 4> expected_vertices_189 = {
      Vec2f(541.56270, 557.20613), Vec2f(442.79386, 541.5627),
      Vec2f(458.43730, 442.79386), Vec2f(557.20613, 458.4373)};
  this->CheckRotation(expected_vertices_189, angle_189);
}

TEST_F(RotateVerticesTest, NegativeAngle) {
  const Angle angle_neg_45 = Angle::FromDegrees(-45.0);
  const std::array<pe2d::Vec2f, 4> expected_vertices_neg_45 = {
      Vec2f(429.28932, 500.0), Vec2f(500.0, 429.28932), Vec2f(570.71067, 500.0),
      Vec2f(500, 570.71069)};
  this->CheckRotation(expected_vertices_neg_45, angle_neg_45);

  const Angle angle_neg_90 = Angle::FromDegrees(-90.0);
  const std::array<Vec2f, 4> expected_vertices_neg_90 = {
      Vec2f(450.0f, 550.0f), Vec2f(450.0f, 450.0f), Vec2f(550.0f, 450.0f),
      Vec2f(550.0f, 550.0f)};
  this->CheckRotation(expected_vertices_neg_90, angle_neg_90);

  const Angle angle_neg_189 = Angle::FromDegrees(-189.0);
  const std::array<Vec2f, 4> expected_vertices_neg_189 = {
      Vec2f(557.20612, 541.56268), Vec2f(458.43732, 557.20612),
      Vec2f(442.79385, 458.43732), Vec2f(541.56268, 442.79385)};
  this->CheckRotation(expected_vertices_neg_189, angle_neg_189);
}

TEST(GetBoxVerticesTest, NonRotatedNonScaled) {
  const Vec2f box_size = Vec2f(100.0, 100.0);
  const Transform box_transform(Vec2f(0.0, 0.0));
  std::array<Vec2f, 4> expected_vertices = {
      Vec2f(50.0, -50.0), Vec2f(-50.0, -50.0), Vec2f(-50.0, 50.0),
      Vec2f(50.0, 50.0)};

  EXPECT_EQ(algo::GetBoxVertices(box_size, box_transform), expected_vertices);
}

TEST(GetBoxVerticesTest, RotatedScaled) {
  const Vec2f box_size = Vec2f(100.0, 100.0);
  const Transform box_transform = Transform(
      Vec2f(500.0, 500.0), Angle::FromDegrees(45.0), Vec2f(1.0, 2.0));
  const std::array<Vec2f, 4> expected_box_vertices = {
      Vec2f(606.06604, 464.644653), Vec2f(535.35535, 393.934),
      Vec2f(393.934, 535.35535), Vec2f(464.64465, 606.06604)};
  const std::array<Vec2f, 4> box_vertices =
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
  const Vec2f proj1 (1.0, 3.0);
  const Vec2f proj2 (3.0, 7.0);
  float overlap1 = 0.0f;
  EXPECT_TRUE(algo::Overlap(proj1, proj2, overlap1));
  EXPECT_FLOAT_EQ(overlap1, 0.0f);

  const Vec2f proj3(1.0f, 3.3f);
  const Vec2f proj4(3.2f, 7.0f);
  float overlap2 = 0.0f;
  EXPECT_TRUE(algo::Overlap(proj3, proj4, overlap2));
  EXPECT_NEAR(overlap2, 0.1f, 0.01f);
}

TEST(OverlapTest, NoOverlap) {
  const Vec2f proj1 = Vec2f(1.0, 3.0);
  const Vec2f proj2 = Vec2f(3.1, 7.0);
  float overlap1 = 0.0;
  EXPECT_FALSE(algo::Overlap(proj1, proj2, overlap1));
  EXPECT_EQ(overlap1, -1.0);

  const Vec2f proj3 = Vec2f(1100.0, 3000.0);
  const Vec2f proj4 = Vec2f(-700.1, -300.0);
  float overlap2 = 0.0;
  EXPECT_FALSE(algo::Overlap(proj3, proj4, overlap2));
  EXPECT_EQ(overlap2, -1.0);
}

TEST(GetBoxAxesTest, NonRotatedNonScaled) {
  const std::array<Vec2f, 4> vertices = {
      Vec2f(450.0, 450.0), Vec2f(550.0, 450.0), Vec2f(550.0, 550.0),
      Vec2f(450.0, 550.0)};
  const std::array<Vec2f, 2> axes = {Vec2f(0.0, -1.0), Vec2f(1.0, 0.0)};
  EXPECT_EQ(algo::GetBoxAxes(vertices), axes);
}

TEST(GetCircleAxisTest, NonRotatedBoxVertices) {
  const std::array<Vec2f, 4> box_vertices = {
      Vec2f(450.0, 450.0), Vec2f(550.0, 450.0), Vec2f(550.0, 550.0),
      Vec2f(450.0, 550.0)};

  const Vec2f circle_center = Vec2f(600.0, 300.0);
  const Vec2f expected_axis = Vec2f(0.31622776, -0.94868332);
  const Vec2f axis = algo::GetCircleAxis(box_vertices, circle_center);
  EXPECT_FLOAT_EQ(axis.x, expected_axis.x);
  EXPECT_FLOAT_EQ(axis.y, expected_axis.y);
}

TEST(GetCircleAxisTest, RotatedBoxVertices) {
  const std::array<Vec2f, 4> box_vertices = {
      Vec2f(535.355286, 393.93396), Vec2f(606.065979, 464.644592),
      Vec2f(464.644714, 606.06604), Vec2f(393.934021, 535.355408)};

  const Vec2f circle_center = Vec2f(600.0, 300.0);
  const Vec2f expected_axis = Vec2f(0.56691653, -0.823775);
  const Vec2f axis = algo::GetCircleAxis(box_vertices, circle_center);
  EXPECT_FLOAT_EQ(axis.x, expected_axis.x);
  EXPECT_FLOAT_EQ(axis.y, expected_axis.y);
}

TEST(ProjectBoxTest, AlignedAxis) {
  const std::array<Vec2f, 4> box_vertices = {
      Vec2f(339.5, 170.0), Vec2f(418.5, 170.0), Vec2f(418.5, 252.0),
      Vec2f(339.5, 252.0)};

  const Vec2f axis1 = Vec2f(1.0, 0.0);
  const Vec2f expected_projection1 = Vec2f(339.5, 418.5);
  const Vec2f projection1 = algo::Project(box_vertices, axis1);
  EXPECT_FLOAT_EQ(projection1.x, expected_projection1.x);
  EXPECT_FLOAT_EQ(projection1.y, expected_projection1.y);

  const Vec2f axis2 = Vec2f(0.0, 1.0);
  const Vec2f expected_projection2 = Vec2f(170.0, 252.0);
  const Vec2f projection2 = algo::Project(box_vertices, axis2);
  EXPECT_FLOAT_EQ(projection2.x, expected_projection2.x);
  EXPECT_FLOAT_EQ(projection2.y, expected_projection2.y);
}

TEST(ProjectBoxTest, NonAlignedAxis) {
  const std::array<Vec2f, 4> box_vertices = {
      Vec2f(339.5, 170.0), Vec2f(418.5, 170.0), Vec2f(418.5, 252.0),
      Vec2f(339.5, 252.0)};

  const Vec2f axis1 = Vec2f(0.05, 0.95);
  const Vec2f expected_projection1 = Vec2f(178.475006, 260.324982);
  const Vec2f projection1 = algo::Project(box_vertices, axis1);
  EXPECT_FLOAT_EQ(projection1.x, expected_projection1.x);
  EXPECT_FLOAT_EQ(projection1.y, expected_projection1.y);

  const Vec2f axis2 = Vec2f(0.21, 0.79);
  const Vec2f expected_projection2 = Vec2f(205.595001, 286.964996);
  const Vec2f projection2 = algo::Project(box_vertices, axis2);
  EXPECT_FLOAT_EQ(projection2.x, expected_projection2.x);
  EXPECT_FLOAT_EQ(projection2.y, expected_projection2.y);
}

TEST(ProjectCircleTest, AlignedAxis) {
  const Vec2f circle_center = Vec2f(70.0, 800.0);
  const float radius = 60.0;
  const Vec2f axis1 = Vec2f(0.0, 1.0);
  const Vec2f expected_projection1 = Vec2f(740.0, 860.0);
  const Vec2f projection1 = algo::ProjectCircle(circle_center, radius, axis1);

  EXPECT_FLOAT_EQ(projection1.x, expected_projection1.x);
  EXPECT_FLOAT_EQ(projection1.y, expected_projection1.y);

  const Vec2f axis2 = Vec2f(1.0, 0.0);
  const Vec2f expected_projection2 = Vec2f(10.0, 130.0);
  const Vec2f projection2 = algo::ProjectCircle(circle_center, radius, axis2);
  EXPECT_FLOAT_EQ(projection2.x, expected_projection2.x);
  EXPECT_FLOAT_EQ(projection2.y, expected_projection2.y);
}

TEST(ProjectCircleTest, NonAlignedAxis) {
  const Vec2f circle_center = Vec2f(500.0, 480.0);
  const float radius = 78.0;
  const Vec2f axis1 = Vec2f(0.5, 0.5);
  const Vec2f expected_projection1 = Vec2f(451.0, 529.0);
  const Vec2f projection1 = algo::ProjectCircle(circle_center, radius, axis1);
  EXPECT_FLOAT_EQ(projection1.x, expected_projection1.x);
  EXPECT_FLOAT_EQ(projection1.y, expected_projection1.y);

  const Vec2f axis2 = Vec2f(0.7, 0.3);
  const Vec2f expected_projection2 = Vec2f(448.76001, 539.23999);
  const Vec2f projection2 = algo::ProjectCircle(circle_center, radius, axis2);

  EXPECT_FLOAT_EQ(projection2.x, expected_projection2.x);
  EXPECT_FLOAT_EQ(projection2.y, expected_projection2.y);
}
} // namespace