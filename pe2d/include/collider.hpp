#pragma once

// local
#include "collision_points.hpp"
#include "rigid_body.hpp"

// std
#include <cassert>

namespace pe2d {
CollisionPoints TestCollison(const RigidBody &a, const RigidBody &b);
/*
    Returns detailed information about a collision beetwen two circles:
    - The depth of penetration between the objects.
    - The normal vector along which the penetration occurred.
    - The contact point where the collision took place.
    - A boolean indicating whether a collision actually occured.
*/
CollisionPoints FindCircleCircleCollision(double radiusA,
                                          Transform circle_transformA,
                                          double radiusB,
                                          Transform circle_transformB);

/*
Returns detailed information about a collision beetwen circle and box:
- The depth of penetration between the objects.
- The normal vector along which the penetration occurred.
- The contact point where the collision took place.
- A boolean indicating whether a collision actually occured.
*/
CollisionPoints FindCircleBoxCollision(double radius,
                                       Transform circle_transform,
                                       Size2d box_size,
                                       Transform box_transform);

CollisionPoints FindBoxCircleCollision(Size2d box_size, Transform box_transform,
                                       double radius,
                                       Transform circle_transform);

/*
Returns detailed information about a collision beetwen two boxes:
- The depth of penetration between the objects.
- The normal vector along which the penetration occurred.
- The contact points where the collision took place.
- A boolean indicating whether a collision actually occured.
*/
CollisionPoints FindBoxBoxCollision(Size2d box_sizeA, Transform box_transformA,
                                    Size2d box_sizeB, Transform box_transformB);

} // namespace pe2d