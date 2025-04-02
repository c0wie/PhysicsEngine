#pragma once

// local
#include "vector2.hpp"

// std
#include <cmath>
#include <numbers>

namespace pe2d::math {
constexpr double INF = std::numeric_limits<double>::infinity();
constexpr double PI = std::numbers::pi;
// (180.0 / PI)
constexpr double RADIANS_TO_DEGREES_SCALAR = 57.295779513082300;
// (PI / 180.0)
constexpr double DEGREES_TO_RADIANS_SCALAR = 0.017453292519943;

template <typename T> constexpr T Length(Vector2<T> a) {
  return std::hypot(a.x, a.y);
}

template <typename T> constexpr double SquaredLength(Vector2<T> a) {
  return a.x * a.x + a.y * a.y;
}

constexpr double Distance(Pos2d a, Pos2d b) {
  const double distanceX = a.x - b.x;
  const double distanceY = a.y - b.y;
  return std::hypot(distanceX, distanceY);
}

constexpr double SquaredDistance(Pos2d a, Pos2d b) {
  const double distanceX = a.x - b.x;
  const double distanceY = a.y - b.y;
  return distanceX * distanceX + distanceY * distanceY;
}

template <typename T> constexpr Vector2<T> Perp(Vector2<T> a) {
  return Vector2<T>(-a.y, a.x);
}

template <typename T> constexpr Vector2<T> Normalize(Vector2<T> a) {
  const double length = Length(a);

  if (length == 0.0) {
    return {};
  }

  return {a.x / length, a.y / length};
}

template <typename T> constexpr double Dot(Vector2<T> a, Vector2<T> b) {
  return a.x * b.x + a.y * b.y;
}

template <typename T> constexpr double Cross(Vector2<T> a, Vector2<T> b) {
  return a.x * b.y - a.y * b.x;
}

constexpr double RadiansToDegrees(double angle_radians) {
  return angle_radians * RADIANS_TO_DEGREES_SCALAR;
}

constexpr double DegreesToRadians(double angle_deegres) {
  return angle_deegres * DEGREES_TO_RADIANS_SCALAR;
}

template <typename T>
constexpr bool NearlyEquel(T a, T b, double abs_error = 0.01) {
  return std::abs(a - b) <= abs_error;
}

template <typename T> 
constexpr bool NearlyEquel(Vector2<T> a, Vector2<T> b, double absError) {
  return SquaredDistance(a, b) < absError * absError;
}
}; // namespace pe2d::math