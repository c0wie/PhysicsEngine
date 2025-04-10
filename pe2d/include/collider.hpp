#pragma once

// local
#include "collision_points.hpp"
#include "rigid_body.hpp"
#include "vector2.hpp"

// std
#include <cassert>

namespace pe2d {
CollisionPoints TestCollison(const CollisionBody *a, const CollisionBody *b);
CollisionPoints TestCollison(const RigidBody *a, const RigidBody *b);
/*
    Returns detailed information about a collision beetwen two circles:
    - The depth of penetration between the objects.
    - The normal vector along which the penetration occurred.
    - The contact point where the collision took place.
    - A boolean indicating whether a collision actually occured.
*/
CollisionPoints FindCircleCircleCollision(float radiusA,
                                          Transform circle_transformA,
                                          float radiusB,
                                          Transform circle_transformB);

/*
Returns detailed information about a collision beetwen circle and box:
- The depth of penetration between the objects.
- The normal vector along which the penetration occurred.
- The contact point where the collision took place.
- A boolean indicating whether a collision actually occured.
*/
CollisionPoints FindCircleBoxCollision(float radius,
                                       Transform circle_transform,
                                       Vec2f box_size,
                                       Transform box_transform);

CollisionPoints FindBoxCircleCollision(Vec2f box_size, Transform box_transform,
                                       float radius,
                                       Transform circle_transform);

/*
Returns detailed information about a collision beetwen two boxes:
- The depth of penetration between the objects.
- The normal vector along which the penetration occurred.
- The contact points where the collision took place.
- A boolean indicating whether a collision actually occured.
*/
CollisionPoints FindBoxBoxCollision(Vec2f box_sizeA, Transform box_transformA,
                                    Vec2f box_sizeB, Transform box_transformB);

} // namespace pe2d