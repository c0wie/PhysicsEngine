#pragma once

#include "Object.hpp"
#include "CollisionPoints.hpp"

namespace phy
{
    class Collision
    {
    public: 
        std::shared_ptr<Object> ObjectA;
        std::shared_ptr<Object> ObjectB;
        CollisionPoints Points;
    };
}