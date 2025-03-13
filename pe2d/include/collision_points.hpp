#pragma once

// local
#include "vector2.hpp"

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
                  Pos2d contact_point2, bool has_collision)
      : Normal(normal), ContactPoint1(contact_point1),
        ContactPoint2(contact_point2), Depth(depth), ContactCount(2),
        HasCollision(has_collision) {}
  CollisionPoints(Vec2d normal, float depth, Pos2d contact_point1,
                  bool has_collision)
      : Normal(normal), ContactPoint1(contact_point1), Depth(depth),
        ContactCount(1), HasCollision(has_collision) {}

public:
  Vec2d Normal{0.0, 0.0}; // normalized length of vector B-A
  Pos2d ContactPoint1{0.0, 0.0};
  Pos2d ContactPoint2{0.0, 0.0};
  float Depth{0.0}; // length of overlap beetwen two objects
  unsigned int ContactCount{0};
  bool HasCollision{false};
};
} // namespace pe2d