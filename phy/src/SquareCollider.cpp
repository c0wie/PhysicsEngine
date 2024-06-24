#include "../headers/SquareCollider.hpp"
#include "../headers/Algo.hpp"
#include <SFML/Graphics.hpp>
namespace phy
{
    SquareCollider::SquareCollider() : 
        SideLength(0.0f) {};

    SquareCollider::SquareCollider(float sideLength) :
        SideLength(sideLength) {};

    SquareCollider::SquareCollider(const SquareCollider &other) :
        SideLength(other.SideLength)
    {}

    SquareCollider::SquareCollider(SquareCollider &&other) noexcept :
       SideLength(other.SideLength)
    {
        other.SideLength = 0.0f;
    }

    SquareCollider& SquareCollider::operator=(const SquareCollider &other)
    {
        if(this == &other)
        {
            return *this;
        }
        SideLength = other.SideLength;
        return *this;
    }

    SquareCollider& SquareCollider::operator=(SquareCollider &&other) noexcept
    {
        if(this == &other)
        {
            return *this;
        }
        SideLength = other.SideLength;
        other.SideLength = 0.0f;
        return *this;
    }

    CollisionPoints SquareCollider::TestCollision(
        const Transform *transform,
        const Collider *collider,
        const Transform *colliderTransform) const 
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }

    CollisionPoints SquareCollider::TestCollision(
        const Transform *transform,
        const CircleCollider *circle,
        const Transform *circleTransform) const 
    {
        return Algo::FindSquareCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints SquareCollider::TestCollision(
        const Transform *transform,
        const SquareCollider *square,
        const Transform *squareTransform) const 
    {
        return Algo::FindSquareSquareCollision(this, transform, square, squareTransform);
    }

    void SquareCollider::Draw(sf::RenderWindow &window, const Transform *transform) const
    {
        sf::RectangleShape square(sf::Vector2f(SideLength, SideLength));
        square.setOrigin(square.getSize() / 2.0f);
        square.setFillColor(sf::Color::Red);
        square.setPosition(transform->GetPosition().x, transform->GetPosition().y);
        window.draw(square);
    }
}