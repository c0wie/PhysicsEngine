// header
#include "grid.hpp"

//local
#include "collision_body.hpp"
#include "rigid_body.hpp"
#include "vector2.hpp"
#include "algo.hpp"

//std
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

namespace pe2d {
Grid::Grid(Vec2f top_left_corner, int rows, int columns, float cell_size)
    : m_TopLeftCorner(top_left_corner), m_Rows(rows), m_Columns(columns),
      m_CellSize(cell_size) {
  if (m_CellSize <= 0.0f) {
    throw std::invalid_argument(
        "[Grid::Grid()] Error: cell_size must be positive (received: " +
        std::to_string(m_CellSize) + ")");
  }
  if (rows <= 0) {
    throw("[Grid::Grid()] Error: number of rows must be positive (received: " +
          std::to_string(rows) + ")\n");
  }
  if (columns <= 0) {
    throw(
        "[Grid::Grid()] Error: number of columns must be positive (received: " +
        std::to_string(rows) + ")\n");
  }
}

void Grid::Update(const std::vector<CollisionBody *> &objects) {
  m_Grid.clear();
  m_Grid.resize(m_Columns);
  const Vec2f bot_right_corner(m_TopLeftCorner.x + m_Columns * m_CellSize, m_TopLeftCorner.y + m_Rows * m_CellSize);
  for (const auto &object : objects) {
    const std::array<Vec2f, 4> bounding_box = object->GetBoundingBox();
    bool inside_grid = false;
    
    for (std::size_t i = 0; i < bounding_box.size(); i++) {
      if(algo::PointInsideRect(m_TopLeftCorner,
        bot_right_corner, bounding_box[i])) {
        inside_grid = true;
        break;
      }
    }
    if (!inside_grid) {
      continue;
    }
    Vector2 object_min = bounding_box[1];
    Vector2 object_max = bounding_box[3];

    // calculate object's coordinates in grid
    int body_coord_minX = static_cast<int>(
        std::floor((object_min.x - m_TopLeftCorner.x) / m_CellSize));
    body_coord_minX = std::clamp(body_coord_minX, 0, (int)m_Columns - 1);
    int body_coord_maxX = static_cast<int>(
        std::floor((object_max.x - m_TopLeftCorner.x) / m_CellSize));
    body_coord_maxX = std::clamp(body_coord_maxX, 0, (int)m_Columns - 1);
    int body_coord_minY = static_cast<int>(
        std::floor((object_min.y - m_TopLeftCorner.y) / m_CellSize));
    body_coord_minY = std::clamp(body_coord_minY, 0, (int)m_Rows - 1);
    int body_coord_maxY = static_cast<int>(
        std::floor((object_max.y - m_TopLeftCorner.y)) / m_CellSize);
    body_coord_maxY = std::clamp(body_coord_maxY, 0, (int)m_Rows - 1);
    
    for (int x = body_coord_minX; x <= body_coord_maxX; x++) {
      if(m_Grid[x].empty()) {
        m_Grid[x].resize(m_Rows);
      }
      for (int y = body_coord_minY; y <= body_coord_maxY; y++) {
        m_Grid[x][y].push_back(object);
      }
    }
  }
}

std::list<std::pair<CollisionBody *, CollisionBody *>> Grid::GetCollisionPairs() const {
  std::unordered_multimap<CollisionBody *, CollisionBody *> checked_pairs;
  std::list<std::pair<CollisionBody *, CollisionBody *>> pairs;

  for(const auto &column : m_Grid) {
    for(const auto &cell : column) {
      for (const auto objA : cell) {
        for (const auto objB : cell) {
          if(objA == objB) {
            break;
          }
          std::pair<CollisionBody *, CollisionBody *> pair;
          if (objA < objB) {
            pair = std::make_pair(objA, objB);
          } else {
            pair = std::make_pair(objB, objA);
          }
          if (!HasBeenChecked(checked_pairs, pair)) {
            pairs.push_back(pair);
            checked_pairs.insert(pair);
          }
        }
      }
    }
  }
  return pairs;
}

bool Grid::HasBeenChecked(
    std::unordered_multimap<CollisionBody *, CollisionBody *> &checked_pairs,
    std::pair<CollisionBody *, CollisionBody *> pair) const {
  auto [first, second] = checked_pairs.equal_range(pair.first);
  for (auto &i = first; i != second; ++i) {
    if (i->second == pair.second) {
      return true;
    }
  }
  return false;
}
} // namespace pe2d