#pragma once


#include "Object.hpp"

namespace phy
{
    class Collision
    {
    public: 
        std::shared_ptr<Object> ObjectA;
        std::shared_ptr<Object> ObjectB;
        CollisionPoints Points;
    public:
        Collision();
        Collision(std::shared_ptr<Object> a, std::shared_ptr<Object> b, CollisionPoints points);
        Collision(const Collision &other);
        Collision(Collision &&other) noexcept;
        Collision& operator=(const Collision &other) ;
        Collision& operator=(Collision &&other) noexcept;
    };
}