// might be colliding with SFML's version of Transform.hpp
#pragma once

#include "Vector2.hpp"
namespace phy
{
    class Transform
    {
    public:
        Vector2 Position;
        Vector2 Scale;
    };
}