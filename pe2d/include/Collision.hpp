#pragma once

#include "RigidObject.hpp"

namespace pe2d
{
    /*
        Struct holding general information about collision:
        - `ObjectA` and `ObjectB`: objects involved in collision
        - `Points`: sturct with detailed information obout collision
    */
    struct Collision
    {
    public:
        Collision(RigidObject &a, RigidObject &b, const CollisionPoints &points) :
            ObjectA(a), ObjectB(b), Points(points) {}
        Collision(const Collision &other) = default;
        Collision(Collision &&other) noexcept = default;
        Collision& operator=(const Collision &other) = default;
        Collision& operator=(Collision &&other) noexcept = default;
    public: 
        RigidObject &ObjectA;
        RigidObject &ObjectB;
        CollisionPoints Points;
    };
}