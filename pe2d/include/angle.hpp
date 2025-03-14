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
private:
  constexpr Angle(double angle_degrees) :
   degrees(angle_degrees) {}
  double degrees{0.0};
};

}