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
        Collision() :
            ObjectA(nullptr), ObjectB(nullptr), Points() {};
        
        Collision(std::shared_ptr<Object> a, std::shared_ptr<Object> b, CollisionPoints points) :
            ObjectA(a), ObjectB(b), Points(points) {};

        Collision(const Collision &other) :
            ObjectA(other.ObjectA), ObjectB(other.ObjectB), Points(other.Points) {};

        Collision(Collision &&other) noexcept:
            ObjectA(other.ObjectA), ObjectB(other.ObjectB), Points(other.Points) 
        {
            other.ObjectA = nullptr;
            other.ObjectB = nullptr;
            other.Points = CollisionPoints();
        };

        Collision& operator=(const Collision &other) 
        {
            if(this == &other)
            {
                return *this;
            }
            ObjectA = other.ObjectA;
            ObjectB = other.ObjectB;
            Points = other.Points;
            return *this;
        }

        Collision& operator=(Collision &&other) noexcept
        {
            if(this == &other)
            {
                return *this;
            }
            ObjectA = other.ObjectA;
            ObjectB = other.ObjectB;
            Points = other.Points;
            other.ObjectA = nullptr;
            other.ObjectB = nullptr;
            other.Points = CollisionPoints();
            return *this;
        }
    };
}