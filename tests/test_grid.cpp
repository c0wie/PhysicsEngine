#include <iostream>
#define private public
#include "grid.hpp"
#define private private
#include "angle.hpp"
#include "rigid_body.hpp"
#include "transform.hpp"
#include "vector2.hpp"
#include <gtest/gtest.h>

namespace {
using namespace pe2d;
class GridTest : public testing::Test {
protected:
  void SetUp() override {
    m_Grid = Grid(Vec2f(0.0f, 0.0f), 5, 5, 20.0f);
    m_Grid.m_Grid.resize(5);
    for(auto &row : m_Grid.m_Grid) {
      row.resize(5);
    }
  }

  void Test_Update(const std::unordered_map<size_t, RigidBody *> &objects,
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
    const auto IDpairs = m_Grid.GetCollisionPairs();
    ASSERT_EQ(IDpairs.size(), expectedIDPairs.size());
    std::cerr << "no assert";
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
  void InsertID(int row, int column, size_t ID) { 
    m_Grid.m_Grid[column][row].push_back(ID);
    std::cerr << "added";
  }

protected:
  Grid m_Grid;
};

TEST_F(GridTest, Update) {
  RigidBody objectA(Circle, Vec2f(19.0, 19.0), {}, 10.0, false, {});
  RigidBody objectB(Box, Vec2f(15.0f, 15.0f),
                    Transform({50.0f, 50.0f}, Angle::FromDegrees(45.0)), 10.0f,
                    false, {});

  const std::unordered_map<size_t, RigidBody *> objects = {{0, &objectA},
                                                           {1, &objectB}};

  const std::vector<Vec2i> expected_taken_cells = {
    Vec2i(1, 1), Vec2i(2, 1),
    Vec2i(3, 1), Vec2i(1, 2),
    Vec2i(2, 2), Vec2i(3, 2),
    Vec2i(1, 3), Vec2i(2, 3),
    Vec2i(3, 3), Vec2i(0, 0)
  };
  
  if(objects.at(0) == nullptr) {
    std::cerr << "objectA are not alive";
  } else if(objects.at(1) == nullptr) {
    std::cerr << "objectB are not alive";
  }
  std::cerr << "objects are alive";
  this->Test_Update(objects, expected_taken_cells);
}

TEST_F(GridTest, GetCollisionPairs) {
  std::list<std::pair<size_t, size_t>> expected_ID_pairs = {std::make_pair(0, 2),
                                                          std::make_pair(1, 2)};
  this->InsertID(0, 0, 0);
  this->InsertID(0, 0, 2);
  this->InsertID(1, 0, 1);
  this->InsertID(1, 0, 2);
  this->Test_GetCollisionPairs(expected_ID_pairs);
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