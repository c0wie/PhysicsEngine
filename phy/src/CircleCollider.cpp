#include "../headers/CircleCollider.hpp"
#include "../headers/Algo.hpp"
#include <SFML/Graphics.hpp>
namespace phy
{
    CircleCollider::CircleCollider() : 
        Radius(0.0f) {};

    CircleCollider::CircleCollider(float radius) :
        Radius(radius) {};

    CircleCollider::CircleCollider(const CircleCollider &other) :
        Radius(other.Radius)
    {}

    CircleCollider::CircleCollider(CircleCollider &&other) noexcept :
        Radius(other.Radius)
    {
        other.Radius = 0.0f;
    }

    CircleCollider& CircleCollider::operator=(const CircleCollider &other)
    {
        if(this == &other)
        {
            return *this;
        }
        Radius = other.Radius;
        return *this;
    }

    CircleCollider& CircleCollider::operator=(CircleCollider &&other) noexcept
    {
        if(this == &other)
        {
            return *this;
        }
        Radius = other.Radius;
        other.Radius = 0.0f;
        return *this;
    }

    CollisionPoints CircleCollider::TestCollision(
        const Transform *transform,
        const Collider *collider,
        const Transform *colliderTransform) const 
    {
        return collider->TestCollision(colliderTransform, this, transform);
    }
    
    CollisionPoints CircleCollider::TestCollision(
        const Transform *transform,
        const CircleCollider *circle,
        const Transform *circleTransform) const 
    {
        return Algo::FindCircleCircleCollision(this, transform, circle, circleTransform);
    }

    CollisionPoints CircleCollider::TestCollision(
        const Transform *transform,
        const SquareCollider *square,
        const Transform *squareTransform) const 
    {
        return Algo::FindCircleSquareCollision(this, transform, square, squareTransform);
    }

    void CircleCollider::Draw(sf::RenderWindow &window, const Transform *transform) const
    {
        sf::CircleShape circle(Radius);
        circle.setOrigin(circle.getRadius() / 2.0f, circle.getRadius() / 2.0f);
        circle.setFillColor(sf::Color::Blue);
        circle.setPosition(transform->GetPosition().x, transform->GetPosition().y);
        window.draw(circle);
    }
}