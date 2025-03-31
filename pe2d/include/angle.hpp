#pragma once 

// local 
#include "math.hpp"

namespace pe2d {
/// \class Angle
/// @brief Represents the absolute orientation of objects
/// @details Stores the angle in degrees internally.
/// @example ../../docs/examples/angle_example.cpp
class Angle {  
public:
  /// @brief Default constructor creates a zero degree angle
  constexpr Angle() = default;

  /// @brief Static constructor creating an Angle from degrees
  /// @param angle - angle value in degrees
  /// @return Angle instance initialized with the given degrees
  static constexpr Angle FromDegrees(double angle) { return Angle(angle); }

  /// @brief Static constructor creating an Angle from radians
  /// @param angle - angle value in radians
  /// @return Angle instance initialized with the given radians
  static constexpr Angle FromRadians(double angle) { return Angle(math::RadiansToDegrees(angle)); }

  /// @brief Returns angle as degrees
  /// @details Performs conversion using the formula: radians = degrees ×
  /// (π/180)
  /// @return Angle as radians
  double AsRadians() const { return math::DegreesToRadians(m_Degrees); }

  /// @brief Returns angle as degrees
  /// @return Angle as degrees
  double AsDegrees() const { return m_Degrees; }

  /// @brief Adds two angles
  /// @param other - Angle to add
  /// @return New angle representing the sum
  constexpr Angle operator+(Angle other) const {
    return FromDegrees(m_Degrees + other.m_Degrees);
  }

  /// @brief Adds another angle to this angle
  /// @param other - Angle to add
  /// @return Reference to this angle
  constexpr Angle &operator+=(Angle other) { return *this = *this + other; }

  /// @brief Subtracts two angles
  /// @param other - Angle to subtract
  /// @return New angle representing the difference
  constexpr Angle operator-(Angle other) const {
    return FromDegrees(m_Degrees - other.m_Degrees);
  }

  /// @brief Subtracts another angle from this angle
  /// @param other - Angle to subtract
  /// @return Reference to this angle
  constexpr Angle &operator-=(Angle other) { return *this = *this - other; }

  /// @brief Equality comparison
  /// @param other - Angle to compare with
  /// @return True if angles are equal, false otherwise
  constexpr bool operator==(Angle other) const {
    return m_Degrees == other.m_Degrees;
  }

private:
  /// @brief Private constructor to enforce use of factory methods
  /// @param angle_degrees - angle in degrees
  explicit constexpr Angle(double angle_degrees) : m_Degrees(angle_degrees) {}
  float m_Degrees{0.0};
};

}