#pragma once

// local
#include "vector2.hpp"
#include <array>

namespace pe2d {
/*
    Struct holding detailed information about collision:
    - `Depth`: The depth of collison.
    - `Normal`: The normal vector along which collision occured.
    - `ContactPoint1`: The first contact point of the collison.
    - `ContactPoint2`: The second contact point of the collision, if applicable.
    - `ContactCount`: The count of valid contact points.
    - `HasCollision`: A boolean indicating whether a collision actually occured.
    The second contact point (`ContactPoint2`) may be invalid or unused in some
   cases, so `contactCount` should be used to determine how many contact points
   are considered valid.
*/
struct CollisionPoints {
public:
  CollisionPoints() = default;

  CollisionPoints(Vec2d normal, float depth, Pos2d contact_point1,
                  Pos2d contact_point2)
      : normal(normal), contact_point1(contact_point1), contact_point2(contact_point2),
        depth(depth), contact_count(2) {}

  CollisionPoints(Vec2d normal, float depth, Pos2d contact_point1)
      : normal(normal), contact_point1(contact_point1), depth(depth),
        contact_count(1) {}
  
  std::array<Pos2d, 2> GetContactPoints() const {return {contact_point1, contact_point2};}
public:
  // normalized length of vector B-A
  Vec2d normal{0.0, 0.0};
  // penetration length
  float depth{0.0};
  // contact points count
  unsigned int contact_count{0};
  Pos2d contact_point1;
  Pos2d contact_point2;
};
} // namespace pe2d