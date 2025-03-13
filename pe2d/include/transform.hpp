#pragma once

// local
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
  constexpr Transform(Pos2d position, double angle_radians, Vec2d scale)
      : position(position), rotation(angle_radians), scale(scale) {
    if (scale.x <= 0.0 || scale.y <= 0.0) {
      throw std::invalid_argument(
          "[Transform::Transform()] Error: scale must be positive (received: " +
          scale.GetString() + ")");
    }
  }
  constexpr Transform(Pos2d position, double angle_radians)
      : position(position), rotation(angle_radians) {}
  constexpr Transform(Pos2d position) : position(position) {}

public:
  constexpr void Move(Vec2d offset) { position += offset; }
  constexpr void Rotate(double angle_radians) { rotation += angle_radians; }

public:
  Pos2d position{0.0, 0.0};
  Vec2d scale{1.0, 1.0};
  double rotation{0.0};
};
} // namespace pe2d