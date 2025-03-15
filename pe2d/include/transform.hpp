#pragma once

// local
#include "angle.hpp"
#include "vector2.hpp"

// std
#include <stdexcept>

namespace pe2d {
/*
    Struct holding basic information about object like:
    - position
    - scale
    - rotation in radians
*/
struct Transform {
public:
  constexpr Transform() = default;
  constexpr Transform(Pos2d position, Angle angle, Vec2d scale)
      : position(position), angle(), scale(scale) {
    if (scale.x <= 0.0 || scale.y <= 0.0) {
      throw std::invalid_argument(
          "[Transform::Transform()] Error: scale must be positive (received: " +
          scale.GetString() + ")");
    }
  }
  constexpr Transform(Pos2d position, Angle angle)
      : position(position), angle(angle) {}
  constexpr Transform(Pos2d position) : position(position) {}

public:
  constexpr void Move(Vec2d offset) { position += offset; }
  constexpr void Rotate(Angle angle_radians) { angle += angle_radians; }

public:
  Pos2d position{0.0, 0.0};
  Vec2d scale{1.0, 1.0};
  Angle angle;
};
} // namespace pe2d