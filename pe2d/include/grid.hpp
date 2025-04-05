#pragma once

// local
#include "rigid_body.hpp"
#include "vector2.hpp"

// std
#include <list>
#include <unordered_map>
#include <vector>

namespace pe2d {
/*
    The Grid class handles broad-phase collision detection by dividing space
   into cells, each containing potential collision pairs of objects. It operates
   by updating the grid with object positions and retrieving a list of potential
   collision pairs for further, more detailed collision detection.
*/
/// @brief nigga
class Grid {
public:
  Grid() = default;
  // size - number of cells in row (x) and columns (y)
  Grid(Vec2f top_left_corner, Vec2i size, float cell_size);

public:
  /*
      Updates the grid with the positions of the objects.
      It places each object's ID in the appropriate grid cell based on its
     position.
  */
  void Update(const std::unordered_map<size_t, RigidBody> &objects);
  /*
      Retrieves a list of unique pairs of object IDs that are potential
     collision pairs. This list is generated based on objects located within the
     same or neighboring cells. The ID of collision pairs which are beeing
     retrived always goes: `std::pair(lesser number, higher number)`. This
     function will throw Segfault if not all off it's rows and columns are
     initialized.
  */
  std::list<std::pair<size_t, size_t>> GetCollisionPairs() const;

protected:
  // Checks if a vertex is inside the grid's bounds.
  constexpr bool Contains(Vec2f vertex) {
    const Vec2f bot_right_corner =
        m_TopLeftCorner + (Vec2f(m_Size.x, m_Size.y) * m_CellSize);
    return vertex.x >= m_TopLeftCorner.x && vertex.x <= bot_right_corner.x &&
           vertex.y >= m_TopLeftCorner.y && vertex.y <= bot_right_corner.y;
  }
  // Checks if a pair of objects has already been checked for collisions.
  bool HasBeenChecked(std::unordered_multimap<size_t, size_t> &checkedPairs,
                      std::pair<size_t, size_t> pair) const;

protected:
  Vec2f m_TopLeftCorner{0.0, 0.0};
  Vec2i m_Size{0, 0};
  float m_CellSize{0.0f};
  std::vector<std::vector<std::vector<size_t>>> m_Grid;
};
} // namespace pe2d