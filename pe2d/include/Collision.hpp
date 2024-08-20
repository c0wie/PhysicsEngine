#pragma once

#include "CollisionObject.hpp"
#include "RigidObject.hpp"

namespace pe2d
{
    /*
        Struct holding detailed information about collision:
        - objects involved in collision
        - CollisionPoints struct
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