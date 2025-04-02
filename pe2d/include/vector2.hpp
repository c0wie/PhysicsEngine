#pragma once

// std
#include <cmath>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>

template <typename T> concept TrivialType = std::is_trivial_v<T>;

namespace pe2d {

/////////////////////////////////////////////////////////////////////
/// @brief Struct represeting two component vector.
/// @details Vector2<T> serves multiple roles in pe2d - not just as a
/// mathematical vector,
///          but also for any case requiring two components. Common uses
///          include:
///          - Representing 2D points (e.g., position)
///          - Representing sizes/dimensions
///          - Storing generic 2D data
/// @tparam T - Type for vector components (e.g float, double, ...).
/////////////////////////////////////////////////////////////////////
template <TrivialType T> struct Vector2 {
public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Default constructor creates zero components.
  /////////////////////////////////////////////////////////////////////
  constexpr Vector2() = default;

  /////////////////////////////////////////////////////////////////////
  /// @brief Constructs a Vector2 with given x and y components.
  /// @param x - the x-component value (must match template type T)
  /// @param y - the y-component value (must match template type T)
  /// @note Both parameters are of type T due to class template parameter.
  ///       For Vector2<float>, both x and y must be float values.
  /////////////////////////////////////////////////////////////////////
  constexpr Vector2(T x, T y) : x(x), y(y) {}

public:
  /////////////////////////////////////////////////////////////////////
  /// @brief Equality comparison operator.
  /// @param other - Vector2 to compare with
  /// @return True if x and y components are the same for both vectors, false
  /// otherwise.
  /////////////////////////////////////////////////////////////////////
  constexpr bool operator==(Vector2 other) const {
    return x == other.x && y == other.y;
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Inequality comparison operator.
  /// @param other - Vector2 to compare with
  /// @return True if at least one components of vectors is different, false
  /////////////////////////////////////////////////////////////////////
  constexpr bool operator!=(Vector2 other) const {
    return x != other.x || y != other.y;
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Vector element-wise addition.
  /// @param other - Vector2 to add
  /// @return New Vector2 representing the sum.
  /////////////////////////////////////////////////////////////////////
  constexpr Vector2 operator+(Vector2 other) const {
    return Vector2{x + other.x, y + other.y};
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Vector element-wise subtraction.
  /// @param other - Vector2 to subtract
  /// @return New Vector2 representing the difference.
  /////////////////////////////////////////////////////////////////////
  constexpr Vector2 operator-(Vector2 other) const {
    return Vector2{x - other.x, y - other.y};
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Vector in-place addition.
  /// @param other - Vector2 to add
  /// @return Reference to this Vector2.
  /////////////////////////////////////////////////////////////////////
  constexpr Vector2 &operator+=(Vector2 other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Vector in-place subtraction.
  /// @param other - Vector2 to subtract
  /// @return Reference to this Vector2.
  /////////////////////////////////////////////////////////////////////
  constexpr Vector2 &operator-=(Vector2 other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Vector element-wise multiplication.
  /// @param other - Vector2 to multiply by element-wise
  /// @return New Vector2 representing element-wise multiplication.
  /////////////////////////////////////////////////////////////////////
  constexpr Vector2 operator*(Vector2 other) const {
    return Vector2{x * other.x, y * other.y};
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Vector scalar multiplication.
  /// @param scalar - Scaling factor of type T
  /// @return Scaled vector where new Vector2 = (x * scalar, y * scalar).
  /////////////////////////////////////////////////////////////////////
  constexpr Vector2 operator*(T scalar) const {
    return Vector2{x * scalar, y * scalar};
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Left-hand-side scalar multiplication.
  /// @param scalar - Scaling factor of type T
  /// @param vec - Vector2 to be scaled
  /// @return New Vector2 equal to vec * scalar.
  /////////////////////////////////////////////////////////////////////
  friend constexpr Vector2 operator*(T scalar, const Vector2 &vec) {
    return vec * scalar;
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Scales this vector in-place.
  /// @param scalar - Scaling factor of type T
  /// @return Reference to this vector.
  /////////////////////////////////////////////////////////////////////
  constexpr Vector2 &operator*=(T scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Vector element-wise division.
  /// @param other - Vector2 to divide by
  /// @return New Vector2 representing element-wise division.
  /// @throws std::invalid::argument if one of other components is zero.
  /// @warning Throws exception on division by zero - consider checking
  /// components before use.
  /////////////////////////////////////////////////////////////////////
  Vector2 operator/(Vector2 other) const {
    if(other.x == T{} || other.y == T{}) {
      throw std::invalid_argument("Division by zero is undefined and at least one of other vector components is zero");
    }
    return Vector2{x / other.x, y / other.y};
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Vector scalar division.
  /// @param scalar - Scaling factor
  /// @return New Vector2 representing scaled vector.
  /// @throws std::invalid::argument if scalar is zero.
  /// @warning Throws exception on division by zero - consider checking scalar
  /// before use.
  /////////////////////////////////////////////////////////////////////
  Vector2 operator/(T scalar) const {
    if(scalar == T{}) {
      throw std::invalid_argument("Division by zero is undefined, scalar is zero");
    }
    return Vector2{x / scalar, y / scalar};
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Vector in-place scalar division.
  /// @param scalar - Scaling factor
  /// @return Reference to this Vector2.
  /// @throws std::invalid::argument if scalar is zero.
  /// @warning Throws exception on division by zero - consider checking scalar
  /// before use.
  /////////////////////////////////////////////////////////////////////
  Vector2 &operator/=(T scalar) {
    if(scalar == T{}) {
      throw std::invalid_argument("Division by zero is undefined, scalar is zero");
    }
    x /= scalar;
    y /= scalar;
    return *this;
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Generates a formatted string representation of Vector2.
  /// @return String in format "[x, y]", where x and y are Vector2 components
  /////////////////////////////////////////////////////////////////////
  std::string GetString() const {
    return "[" + std::to_string(x) + ", " + std::to_string(y) + "]";
  }

  /////////////////////////////////////////////////////////////////////
  /// @brief Stream insertion operator for Vector2.
  /// @tparam Y Component type (automatically deduced)
  /// @param os - Output stream to write to
  /// @param v - Vector2 to serialize
  /// @return Reference to the output stream.
  /////////////////////////////////////////////////////////////////////
  template <typename Y>
  friend std::ostream &operator<<(std::ostream &os, const Vector2<T> &v);

public:
  T x{T{}};
  T y{T{}};
};
template <typename Y>
inline std::ostream &operator<<(std::ostream &os, const Vector2<Y> &v) {
  os << "[" << v.x << ", " << v.y << "]";
  return os;
}
using Vec2i = Vector2<int>;
using Vec2d = Vector2<double>;
using Pos2d = Vector2<double>;
using Size2d = Vector2<double>;
using Size2i = Vector2<int>;
} // namespace pe2d