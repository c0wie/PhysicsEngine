#pragma once

#include "RigidObject.hpp"

namespace pe2d
{
    /*
        Struct holding general information about collision:
        - `ObjectA` and `ObjectB`: objects involved in collision as non owning pointers
        - `Points`: sturct with detailed information obout collision
    */
    struct Collision
    {
    public:
        Collision(RigidObject &a, RigidObject &b, const CollisionPoints &points) :
            Points(points), ObjectA(a), ObjectB(b) {}
    public: 
        CollisionPoints Points;
        RigidObject &ObjectA;
        RigidObject &ObjectB;
    };
}