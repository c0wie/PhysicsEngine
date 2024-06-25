#pragma once

#include <memory>
#include "CollisionPoints.hpp"
#include "Transform.hpp"
#include <SFML/Graphics.hpp>


namespace phy
{
    class SquareCollider;
    class CircleCollider;
    
    class Collider 
    {
    public:
        virtual CollisionPoints TestCollision(
            const Transform *transform,
            const Collider *collider,
            const Transform *colliderTransform) const = 0;
        
        virtual CollisionPoints TestCollision(
            const Transform *transform,
            const CircleCollider *circle,
            const Transform *circleTransform) const = 0;
        
        virtual CollisionPoints TestCollision(
            const Transform *transform,
            const SquareCollider *square,
            const Transform *squareTransform) const = 0;
        
        virtual void Draw(sf::RenderWindow &window, const Transform *transform)const = 0;
    };
}