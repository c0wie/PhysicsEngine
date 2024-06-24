#pragma once


#include "CollisionObject.hpp"

namespace phy
{
    class Collision
    {
    public: 
        std::shared_ptr<CollisionObject> ObjectA;
        std::shared_ptr<CollisionObject> ObjectB;
        CollisionPoints Points;
    public:
        Collision();
        Collision(std::shared_ptr<CollisionObject> a, std::shared_ptr<CollisionObject> b, CollisionPoints points);
        Collision(const Collision &other);
        Collision(Collision &&other) noexcept;
        Collision& operator=(const Collision &other) ;
        Collision& operator=(Collision &&other) noexcept;
    };
}