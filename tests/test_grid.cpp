#include "angle.hpp"
#include "grid.hpp"
#include "rigid_body.hpp"
#include "transform.hpp"
#include "vector2.hpp"
#include <gtest/gtest.h>

namespace {
using namespace pe2d;
class GridTestable : public Grid {
public:
  GridTestable() : Grid() {}
  GridTestable &operator=(const Grid &other) {
    if (this == &other) {
      return *this;
    }
    Grid::operator=(other);
    return *this;
  }

public:
  using Grid::Contains;
  using Grid::HasBeenChecked;
  using Grid::m_Grid;
};

class GridTest : public testing::Test {
protected:
  void SetUp() override {
    m_Grid = Grid(Vec2f(0.0f, 0.0f), Vec2i(5, 5), 20.0f);
    m_Grid.m_Grid.resize(5);
    for (int i = 0; i < 5; i++) {
      m_Grid.m_Grid[i].resize(5);
    }
  }
  void Test_Contains(Vec2f point, bool expected_result) {
    const bool result = m_Grid.Contains(point);
    EXPECT_EQ(result, expected_result);
  }

  void Test_Update(const std::unordered_map<size_t, RigidBody> &objects,
                   const std::vector<Vec2i> &expected_taken_cells) {

    m_Grid.Update(objects);
    for (int i = 0; i < expected_taken_cells.size(); i++) {
      const int x = expected_taken_cells[i].x;
      const int y = expected_taken_cells[i].y;
      const bool isEmpty = m_Grid.m_Grid[x][y].empty();
      EXPECT_EQ(isEmpty, false);
    }
  }

  void Test_GetCollisionPairs(
      std::list<std::pair<size_t, size_t>> &expectedIDPairs) {
    auto IDpairs = m_Grid.GetCollisionPairs();
    ASSERT_EQ(IDpairs.size(), expectedIDPairs.size());

    auto it1 = IDpairs.begin();
    auto it2 = expectedIDPairs.begin();
    while (it1 != IDpairs.end()) {
      EXPECT_EQ(it1->first, it2->first);
      EXPECT_EQ(it1->second, it2->second);
      it1++;
      it2++;
    }
  }
  void
  Test_HasBeenChecked(std::unordered_multimap<size_t, size_t> &checkedPairs,
                      std::pair<size_t, size_t> pair, float expectedResult) {
    const bool result = m_Grid.HasBeenChecked(checkedPairs, pair);
    EXPECT_EQ(result, expectedResult);
  }
  void InsertID(int x, int y, size_t ID) { m_Grid.m_Grid[y][x].push_back(ID); }

protected:
  GridTestable m_Grid;
};

TEST_F(GridTest, ContainsPoint) {
  const Vec2f point1 = Vec2f(50.0, 50.0);
  const Vec2f point2 = Vec2f(0.0, 100.0);
  this->Test_Contains(point1, true);
  this->Test_Contains(point2, true);
}

TEST_F(GridTest, NotContainsPoint) {
  const Vec2f point1 = Vec2f(-50.0, 50.0);
  const Vec2f point2 = Vec2f(-1.0, 100.1);
  this->Test_Contains(point1, false);
  this->Test_Contains(point2, false);
}

TEST_F(GridTest, Update) {
  const std::unordered_map<size_t, RigidBody> objects = {
      {0,
       RigidBody(0, Circle, Vec2f(19.0, 19.0), Transform(), 10.0, false, {})},
      {1, RigidBody(1, Box, Vec2f(15.0f, 15.0f),
                    Transform({50.0f, 50.0f}, Angle::FromDegrees(45.0)), 10.0,
                    false, {})}};

  const std::vector<Vec2i> expected_taken_cells = {
      Vec2i(1.0, 1.0), Vec2i(2.0, 1.0), Vec2i(3.0, 1.0), Vec2i(1.0, 2.0),
      Vec2i(2.0, 2.0), Vec2i(3.0, 2.0), Vec2i(1.0, 3.0), Vec2i(2.0, 3.0),
      Vec2i(3.0, 3.0), Vec2i(0.0, 0.0)};
  this->Test_Update(objects, expected_taken_cells);
}

TEST_F(GridTest, GetCollisionPairs) {
  std::list<std::pair<size_t, size_t>> expectedIDPairs = {std::make_pair(0, 2),
                                                          std::make_pair(1, 2)};
  // row | column | ID
  this->InsertID(0, 0, 0);
  this->InsertID(0, 0, 2);
  this->InsertID(1, 0, 1);
  this->InsertID(1, 0, 2);
  this->Test_GetCollisionPairs(expectedIDPairs);
}

TEST_F(GridTest, HasBeenChecked) {
  std::unordered_multimap<size_t, size_t> checkedPairs = {
      {1, 2}, {4, 6}, {0, 2}, {6, 9}};
  this->Test_HasBeenChecked(checkedPairs, std::make_pair(1, 2), true);
}

TEST_F(GridTest, HasNotBeenChecked) {
  std::unordered_multimap<size_t, size_t> checkedPairs = {
      {1, 2}, {4, 6}, {0, 2}, {6, 9}};
  this->Test_HasBeenChecked(checkedPairs, std::make_pair(4, 8), false);
}
} // namespace