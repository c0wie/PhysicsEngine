//local
#include "../../pe2d/include/angle.hpp"
#include "../../pe2d/include/math.hpp"

//std
#include <iostream>

int main() {
  pe2d::Angle angle1 = pe2d::Angle::FromDegrees(30.0);
  pe2d::Angle angle2 = pe2d::Angle::FromRadians(pe2d::math::PI / 6.0); // 30 degrees

  std::cout << "Angle 1(degrees): " << angle1.AsDegrees() << std::endl;
  std::cout << "Angle 2(radians): " << angle2.AsDegrees() << std::endl;
  
  if (angle1 == angle2) {
    std::cout << "Both angles are equal!" << std::endl;
  }
}