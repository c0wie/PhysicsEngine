#pragma once 

// local 
#include "math.hpp"

namespace pe2d {
class Angle {  
public:
  constexpr Angle() = default;
  static constexpr Angle FromDegrees(double angle) { return Angle(angle); }
  static constexpr Angle FromRadians(double angle) { return Angle(math::RadiansToDegrees(angle)); }

  double AsRadians() const {return math::DegreesToRadians(degrees);}
  double AsDegrees() const {return degrees;}

  constexpr Angle operator+(Angle other) const {
    return FromDegrees(degrees + other.degrees);
  }
  constexpr Angle &operator+=(Angle other) { return *this = *this + other; }
  constexpr Angle operator-(Angle other) const {
    return FromDegrees(degrees - other.degrees);
  }
  constexpr Angle &operator-=(Angle other) { return *this = *this - other; }
  constexpr bool operator==(Angle other) const {
    return degrees == other.degrees;
  }

private:
  explicit constexpr Angle(double angle_degrees) : degrees(angle_degrees) {}
  double degrees{0.0};
};

}