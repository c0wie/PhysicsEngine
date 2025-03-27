// header
#include "grid.hpp"
#include "rigid_body.hpp"
#include "vector2.hpp"
#include <stdexcept>
#include <string>

namespace pe2d {
Grid::Grid(Pos2d top_left_corner, Size2i size, float cell_size)
    : m_TopLeftCorner(top_left_corner), m_Size(size), m_CellSize(cell_size) {
  if (m_CellSize <= 0.0f) {
    throw std::invalid_argument(
        "[Grid::Grid()] Error: cell_size must be positive (received: " +
        std::to_string(m_CellSize) + ")");
  }
  if (size.x <= 0.0 || size.y <= 0.0) {
    throw("[Grid::Grid()] Error: size must be positive (received: " +
          size.GetString() + ")\n");
  }
}

void Grid::Update(const std::unordered_map<size_t, RigidBody> &objects) {
  m_Grid.clear();
  m_Grid.resize(m_Size.y);
  for (const auto &object : objects) {
    const std::array<Pos2d, 4> boundingBox = object.second.GetBoundingBox();
    bool inside_grid = false;
    for (int i = 0; i < 4; i++) {
      if (Contains(boundingBox[i])) {
        inside_grid = true;
        break;
      }
    }
    if (!inside_grid) {
      continue;
    }
    Vector2 object_min = boundingBox[1];
    Vector2 object_max = boundingBox[3];

    // calculate object's coordinates in grid
    int body_coord_minX = static_cast<int>(
        std::floor((object_min.x - m_TopLeftCorner.x) / m_CellSize));
    body_coord_minX = std::clamp(body_coord_minX, 0, (int)m_Size.x - 1);
    int body_coord_maxX = static_cast<int>(
        std::floor((object_max.x - m_TopLeftCorner.x) / m_CellSize));
    body_coord_maxX = std::clamp(body_coord_maxX, 0, (int)m_Size.x - 1);
    int body_coord_minY = static_cast<int>(
        std::floor((object_min.y - m_TopLeftCorner.y) / m_CellSize));
    body_coord_minY = std::clamp(body_coord_minY, 0, (int)m_Size.y - 1);
    int body_coord_maxY = static_cast<int>(
        std::floor((object_max.y - m_TopLeftCorner.y)) / m_CellSize);
    body_coord_maxY = std::clamp(body_coord_maxY, 0, (int)m_Size.y - 1);

    for (int y = body_coord_minY; y <= body_coord_maxY; y++) {
      auto &row = m_Grid[y];
      if (row.empty()) {
        row.resize(m_Size.x);
      }
      for (int x = body_coord_minX; x <= body_coord_maxX; x++) {
        row[x].push_back(object.first);
      }
    }
  }
}

std::list<std::pair<size_t, size_t>> Grid::GetCollisionPairs() const {
  std::unordered_multimap<size_t, size_t> checked_pairs;
  std::list<std::pair<size_t, size_t>> pairs;

  for (const auto &row : m_Grid) {
    for (const auto &cell : row) {
      for (const auto &elementA : cell) {
        for (const auto &elementB : cell) {
          if (elementA == elementB) {
            break;
          }
          std::pair<size_t, size_t> pair;
          if (elementA < elementB) {
            pair = std::make_pair(elementA, elementB);
          } else {
            pair = std::make_pair(elementB, elementA);
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
    std::unordered_multimap<size_t, size_t> &checked_pairs,
    std::pair<size_t, size_t> pair) const {
  auto [first, second] = checked_pairs.equal_range(pair.first);
  for (auto &i = first; i != second; ++i) {
    if (i->second == pair.second) {
      return true;
    }
  }
  return false;
}
} // namespace pe2d