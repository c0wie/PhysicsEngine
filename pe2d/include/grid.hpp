#pragma once

// local
#include "collision_body.hpp"
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
/// @details Grid strores NON owning pointers to objects. It is NOT responsible
///          for keeping them valid.
/////////////////////////////////////////////////////////////////////
class Grid {
public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Default constructor.
  /// @details This constructor initializes all members to 0.
  /////////////////////////////////////////////////////////////////////
  Grid() = default;
  /////////////////////////////////////////////////////////////////////
  /// @brief Constructs the grid with its attributes.
  /// @param top_left_corner starting point of the grid
  /// @param rows number of rows
  /// @param columns number of columns
  /// @param cell_size size of single cell, cell must be squared
  /// @throws std::invalid_argument if rows is non positive.
  /// @throws std::invalid_argument if columns is non positive.
  /// @throws std::invalid_argument if cell_size is non positive.
  /////////////////////////////////////////////////////////////////////
  Grid(Vec2f top_left_corner, int rows, int columns, float cell_size);

public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Place objects in appropriate cells.
  /// @details Clear all the cells from previous update call. To avoid
  ///          fake old collisions calls.
  /// @param objects  vector of collision body pointers
  /////////////////////////////////////////////////////////////////////
  void Update(const std::vector<CollisionBody *> &objects);

  /////////////////////////////////////////////////////////////////////
  /// @brief Group all possible collisions into collision pairs.
  /// @return List of potential collision pairs.
  /////////////////////////////////////////////////////////////////////
  std::list<std::pair<CollisionBody *, CollisionBody *>> GetCollisionPairs() const;

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
  bool HasBeenChecked(std::unordered_multimap<CollisionBody *, CollisionBody *> &checked_pairs,
                      std::pair<CollisionBody *, CollisionBody *> pair) const;

private:
  Vec2f m_TopLeftCorner{0.0, 0.0};
  unsigned int m_Rows{0};
  unsigned int m_Columns{0};
  float m_CellSize{0.0f};
  std::vector<std::vector<std::vector<CollisionBody *>>> m_Grid;
};
} // namespace pe2d