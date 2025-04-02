#pragma once

// local
#include "angle.hpp"
#include "vector2.hpp"

// std
#include <stdexcept>

namespace pe2d {
/////////////////////////////////////////////////////////////////////
/// @brief Struct containing position, angle and scale.
/// @details Transform covers properties which could be modified
///          using linear transformations.
/////////////////////////////////////////////////////////////////////
struct Transform {
public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Default constructor, sets position to (0.0, 0.0), scale to
  /// (1.0, 1.0) and angle to 0.
  /////////////////////////////////////////////////////////////////////
  constexpr Transform() = default;

  /////////////////////////////////////////////////////////////////////
  /// @brief Constructs Transform with given position, angle and scale.
  /// @param position - 2D position of the Transform
  /// @param angle - The rotation angle the Transform
  /// @param scale - Scaling factors in x and y dimensions (must be positive)
  /// @throws std::invalid_argument when any of scale components is less or
  /// equel to 0.0.
  /////////////////////////////////////////////////////////////////////
  constexpr Transform(Pos2d position, Angle angle, Vec2d scale)
      : position(position), angle(angle), scale(scale) {
    if (scale.x <= 0.0 || scale.y <= 0.0) {
      throw std::invalid_argument(
          "[Transform::Transform()] Error: scale must be positive (received: " +
          scale.GetString() + ")");
    }
  }
  /////////////////////////////////////////////////////////////////////
  /// @brief Constructs Transform with given position and angle, scale is set to
  /// default value.
  /// @param position - 2D position of the Transform
  /// @param angle - The rotation angle the Transform
  /////////////////////////////////////////////////////////////////////
  constexpr Transform(Pos2d position, Angle angle)
      : position(position), angle(angle) {}

  /////////////////////////////////////////////////////////////////////
  /// @brief Construct Transform with given position, angle and scale are set to
  /// default values.
  /// @param position - 2D position of the Transform
  /////////////////////////////////////////////////////////////////////
  constexpr Transform(Pos2d position) : position(position) {}

public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Adds offset to position.
  /// @param offset - Change of position
  /////////////////////////////////////////////////////////////////////
  constexpr void Move(Vec2d offset) { position += offset; }

  /////////////////////////////////////////////////////////////////////
  /// @brief Adds angle_offset to angle.
  /// @param angle_offset - Change of angle
  /////////////////////////////////////////////////////////////////////
  constexpr void Rotate(Angle angle_offset) { angle += angle_offset; }

public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Position in 2D space.
  /////////////////////////////////////////////////////////////////////
  Pos2d position{0.0, 0.0};
  /////////////////////////////////////////////////////////////////////
  /// @brief Scaling factors in x and y dimensions (must be positive).
  /////////////////////////////////////////////////////////////////////
  Vec2d scale{1.0, 1.0};

  /////////////////////////////////////////////////////////////////////
  /// @brief Rotation angle.
  /////////////////////////////////////////////////////////////////////
  Angle angle;
};
} // namespace pe2d