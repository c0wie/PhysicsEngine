#pragma once

// local
#include "rigid_body.hpp"
#include "vector2.hpp"

// std
#include <list>
#include <unordered_map>
#include <vector>

namespace pe2d {
/////////////////////////////////////////////////////////////////////
/// @class Grid
/// @brief 2D array-like structure, performing broad-phase collision detection,
///        which seperates given area into smaller chunks and look for potential
///        collisions paris in one chunk at a time.
/// @details Grid only stores rigid body ID and IS NOT RESPONISBLE for
///          keeping rigid object with ID it stores alive. The rigid object
///          belong to the cell if any of its bounding box vertices overlap
///          with cell area. One rigid body CAN be assinged to multiple cells.
/////////////////////////////////////////////////////////////////////
class Grid {
public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Default constructor initializing Grid into state where it
  ///        has zero area.
  /////////////////////////////////////////////////////////////////////
  Grid() = default;
  /////////////////////////////////////////////////////////////////////
  /// @brief Constructor creating Grid with given parameters. From this
  ///        point Grid is working normally.
  /// @param top_left_corner - Position of the grid, unlike rest of the
  ///        classed Grid's postion is in top left corner
  /// @param rows - number of rows
  /// @param columns - number of columns
  /// @param cell_size - size of single cell, cell must be squared
  /// @throws std::invalid_argument if rows is non positive.
  /// @throws std::invalid_argument if columns is non positive.
  /// @throws std::invalid_argument if cell_size is non positive.
  /////////////////////////////////////////////////////////////////////
  Grid(Vec2f top_left_corner, int rows, int columns, float cell_size);

public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Place rigid body IDs in appropriate cells.
  /// @details Clears m_Grid information from previous Update call before
  ///          inserting new info to it.
  /// @param objects - data structure containing rigid_bodies
  /////////////////////////////////////////////////////////////////////
  void Update(const std::unordered_map<size_t, RigidBody *> &objects);

  /////////////////////////////////////////////////////////////////////
  /// @brief Group all possible collisions into collision pairs. Collision pairs
  ///        are be formed only inside the same grid cell.
  /// @details Collision pair format is std::pair(lesser id, higher id).
  /// @return List of potential collision pairs.
  /////////////////////////////////////////////////////////////////////
  std::list<std::pair<size_t, size_t>> GetCollisionPairs() const;

private:
  /////////////////////////////////////////////////////////////////////
  /// @brief Checks whether pair already has been formed to avoid
  ///        doubling the same potentatial collision pair.
  /// @param checked_pairs - all previously created paris
  /// @param pair - new pair 
  /// @return True if the pair (or its reverse) already exists in `checked_pairs`,
  ///         False otherwise.
  /// @note Pairs are internally normalized (sorted) before checking to ensure
  ///       order-insensitive comparison.
  /// @warning Modifies `checked_pairs` if the new pair is inserted.
  /////////////////////////////////////////////////////////////////////
  bool HasBeenChecked(std::unordered_multimap<size_t, size_t> &checked_pairs,
                      std::pair<size_t, size_t> pair) const;

private:
  Vec2f m_TopLeftCorner{0.0, 0.0};
  unsigned int m_Rows{0};
  unsigned int m_Columns{0};
  float m_CellSize{0.0f};
  std::vector<std::vector<std::vector<size_t>>> m_Grid;
};
} // namespace pe2d