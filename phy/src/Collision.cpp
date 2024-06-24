#include "../headers/Collision.hpp"

namespace phy
{
    Collision::Collision() :
        ObjectA(nullptr), ObjectB(nullptr), Points() {};
    
    Collision::Collision(std::shared_ptr<CollisionObject> a, std::shared_ptr<CollisionObject> b, CollisionPoints points) :
        ObjectA(a), ObjectB(b), Points(points) {};

    Collision::Collision(const Collision &other) :
        ObjectA(other.ObjectA), ObjectB(other.ObjectB), Points(other.Points) {};

    Collision::Collision(Collision &&other) noexcept:
        ObjectA(other.ObjectA), ObjectB(other.ObjectB), Points(other.Points) 
    {
        other.ObjectA = nullptr;
        other.ObjectB = nullptr;
        other.Points = CollisionPoints();
    };

    Collision& Collision::operator=(const Collision &other) 
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

    Collision& Collision::operator=(Collision &&other) noexcept
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
}