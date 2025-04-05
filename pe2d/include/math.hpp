#pragma once

// local
#include "vector2.hpp"

// std
#include <cmath>
#include <numbers>

namespace pe2d::math {
constexpr float INF = std::numeric_limits<float>::infinity();
constexpr float PI = std::numbers::pi;
constexpr float RADIANS_TO_DEGREES_SCALAR = 180.0f / PI;
constexpr float DEGREES_TO_RADIANS_SCALAR = PI / 180.0f;

template <typename T> constexpr T Length(Vector2<T> a) {
  return std::hypot(a.x, a.y);
}

template <typename T> constexpr float SquaredLength(Vector2<T> a) {
  return a.x * a.x + a.y * a.y;
}

constexpr float Distance(Vec2f a, Vec2f b) {
  const float distanceX = a.x - b.x;
  const float distanceY = a.y - b.y;
  return std::hypot(distanceX, distanceY);
}

constexpr float SquaredDistance(Vec2f a, Vec2f b) {
  const float distanceX = a.x - b.x;
  const float distanceY = a.y - b.y;
  return distanceX * distanceX + distanceY * distanceY;
}

template <typename T> constexpr Vector2<T> Perp(Vector2<T> a) {
  return Vector2<T>(-a.y, a.x);
}

template <typename T> constexpr Vector2<T> Normalize(Vector2<T> a) {
  const float length = Length(a);

  if (length == 0.0) {
    return {};
  }

  return {a.x / length, a.y / length};
}

template <typename T> constexpr float Dot(Vector2<T> a, Vector2<T> b) {
  return a.x * b.x + a.y * b.y;
}

template <typename T> constexpr float Cross(Vector2<T> a, Vector2<T> b) {
  return a.x * b.y - a.y * b.x;
}

constexpr float RadiansToDegrees(float angle_radians) {
  return angle_radians * RADIANS_TO_DEGREES_SCALAR;
}

constexpr float DegreesToRadians(float angle_deegres) {
  return angle_deegres * DEGREES_TO_RADIANS_SCALAR;
}

template <typename T>
constexpr bool NearlyEquel(T a, T b, float abs_error = 0.01) {
  return std::abs(a - b) <= abs_error;
}

template <typename T> 
constexpr bool NearlyEquel(Vector2<T> a, Vector2<T> b, float absError) {
  return SquaredDistance(a, b) < absError * absError;
}
}; // namespace pe2d::math