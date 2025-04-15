// local
#include "vector2.hpp"
#include "collision_body.hpp"
#define private public
#include "grid.hpp"
#define private private

// lib
// gtest
#include <gtest/gtest.h>

//std
#include <vector>

namespace {
using namespace pe2d;

TEST(GridTest, Update) {
  Grid grid(Vec2f(0.0f, 0.0f), 5, 5, 20.0f);
  CollisionBody objectA(Circle, Vec2f(19.0, 19.0), {}, false);
  CollisionBody objectB(Box, Vec2f(9.0f, 9.0f), {30.0f, 30.0f}, false);
  const std::vector<CollisionBody *> objects = {&objectA, &objectB};
  const std::vector<Vec2i> expected_taken_cells = {
    {0, 0},
    {1, 1},
  };

  grid.Update(objects);
  for (int i = 0; i < expected_taken_cells.size(); i++) {
    const int x = expected_taken_cells[i].x;
    const int y = expected_taken_cells[i].y;
    const bool is_empty = grid.m_Grid[x][y].empty();
    EXPECT_EQ(is_empty, false);
  }
}

TEST(GridTest, GetCollisionPairs) {
  Grid grid(Vec2f(0.0f, 0.0f), 5, 5, 20.0f);
  grid.m_Grid.resize(grid.m_Columns);
  CollisionBody objectA(Circle, {15.0f, 15.0f}, {}, false);
  CollisionBody objectB(Box, {15.0f, 15.0f}, {50.0f, 50.0f}, false);
  CollisionBody objectC(Box, {15.0f, 15.0f}, {50.0f, 50.0f}, false);
  std::list<std::pair<CollisionBody *, CollisionBody *>> expected_pairs = {
  {&objectA, &objectC},
  {&objectB, &objectC}
  };
  for(auto &col : grid.m_Grid) {
    col.resize(grid.m_Rows);
  }
  grid.m_Grid[0][0].push_back(&objectA);
  grid.m_Grid[0][0].push_back(&objectC);
  grid.m_Grid[1][0].push_back(&objectB);
  grid.m_Grid[1][0].push_back(&objectC);
  const auto pairs = grid.GetCollisionPairs();
  auto it1 = pairs.begin();
  auto it2 = expected_pairs.begin();
  while (it1 != pairs.end() || it2 != expected_pairs.end()) {
    EXPECT_EQ(it1->first, it2->first);
    EXPECT_EQ(it1->second, it2->second);
    it1++;
    it2++;
  }
}

TEST(GridTest, HasBeenChecked_False) {
  Grid grid(Vec2f(0.0f, 0.0f), 5, 5, 20.0f);
  CollisionBody objectA(Circle, Vec2f(19.0, 19.0), {}, false);
  CollisionBody objectB(Box, Vec2f(15.0f, 15.0f), {50.0f, 50.0f}, false);
  CollisionBody objectC(Box, Vec2f(15.0f, 15.0f), {50.0f, 50.0f}, false);
  std::unordered_multimap<CollisionBody *, CollisionBody *> checked_pairs = {
    {&objectB, &objectC},
    {&objectB, &objectA}
  };
  EXPECT_FALSE(grid.HasBeenChecked(checked_pairs, {&objectA, &objectC}));
}

TEST(GridTest, HasBeenChecked_True) {
  Grid grid(Vec2f(0.0f, 0.0f), 5, 5, 20.0f);
  CollisionBody objectA(Circle, Vec2f(19.0, 19.0), {}, false);
  CollisionBody objectB(Box, Vec2f(15.0f, 15.0f), {50.0f, 50.0f}, false);
  CollisionBody objectC(Box, Vec2f(15.0f, 15.0f), {50.0f, 50.0f}, false);
  std::unordered_multimap<CollisionBody *, CollisionBody *> checked_pairs = {
    {&objectB, &objectC},
    {&objectA, &objectB}
  };
  EXPECT_TRUE(grid.HasBeenChecked(checked_pairs, {&objectA, &objectB}));
}
} // namespace