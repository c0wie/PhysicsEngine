#pragma once

// std
#include <cmath>
#include <ostream>
#include <string>
#include <type_traits>

template <typename T> concept TrivialType = std::is_trivial_v<T>;

namespace pe2d {
// Basic struct for manipulating 2-dimensional vectors
template <TrivialType T> struct Vector2 {
public:
  constexpr Vector2() = default;
  constexpr Vector2(T x, T y) : x(x), y(y) {}

public:
  constexpr bool operator==(Vector2 other) const {
    return x == other.x && y == other.y;
  }
  constexpr bool operator!=(Vector2 other) const {
    return x != other.x && y != other.y;
  }
  constexpr Vector2 operator+(Vector2 other) const {
    return Vector2{x + other.x, y + other.y};
  }
  constexpr Vector2 operator-(Vector2 other) const {
    return Vector2{x - other.x, y - other.y};
  }
  constexpr Vector2 &operator+=(Vector2 other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  constexpr Vector2 &operator-=(Vector2 other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  constexpr Vector2 operator*(Vector2 other) const {
    return Vector2{x * other.x, y * other.y};
  }
  constexpr Vector2 operator*(double scalar) const {
    return Vector2{x * scalar, y * scalar};
  }
  friend constexpr Vector2 operator*(double scalar, const Vector2 &vec) {
    return vec * scalar;
  }
  constexpr Vector2 &operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  constexpr Vector2 operator/(Vector2 other) const {
    return Vector2{x / other.x, y / other.y};
  }
  constexpr Vector2 operator/(double scalar) const {
    return Vector2{x / scalar, y / scalar};
  }
  constexpr Vector2 &operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  }
  std::string GetString() const {
    return "[" + std::to_string(x) + ", " + std::to_string(y) + "]";
  }
  template <typename Y>
  friend std::ostream &operator<<(std::ostream &os, const Vector2<Y> &v);

public:
  T x{T{}};
  T y{T{}};
};
template <typename Y>
inline std::ostream &operator<<(std::ostream &os, const Vector2<Y> &v) {
  os << "[" << v.x << ", " << v.y << "]";
  return os;
};
using Vec2i = Vector2<int>;
using Vec2d = Vector2<double>;
using Pos2d = Vector2<double>;
using Size2d = Vector2<double>;
using Size2i = Vector2<int>;
} // namespace pe2d