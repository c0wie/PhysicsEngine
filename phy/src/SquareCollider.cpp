#include "../headers/Collider.hpp"
#include "../headers/Algo.hpp"
#include <SFML/Graphics.hpp>
namespace phy
{
    SquareCollider::SquareCollider(float sideLength)
    {
        SetSideLength(sideLength);
    }

    SquareCollider::SquareCollider(SquareCollider &&other) noexcept :
       m_SideLength(other.m_SideLength)
    {
        other.m_SideLength = 0.0f;
    }

    SquareCollider& SquareCollider::operator=(SquareCollider &&other) noexcept
    {
        if(this == &other)
        {
            return *this;
        }
        m_SideLength = other.m_SideLength;
        other.m_SideLength = 0.0f;
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

    float SquareCollider::GetSideLength() const
    {
        return m_SideLength;
    }

    void SquareCollider::SetSideLength(float sideLength)
    {
        if(sideLength <= 0.0f)
        {
            ASSERT("SideLenth must be grater than 0");
        }
        m_SideLength = sideLength;
    }
    
    void SquareCollider::Draw(sf::RenderWindow &window, const Transform *transform) const
    {
        sf::RectangleShape square(sf::Vector2f(m_SideLength, m_SideLength));
        square.setOrigin(square.getSize() / 2.0f);
        square.setFillColor(sf::Color::Red);
        square.setPosition(transform->position.x, transform->position.y);
        window.draw(square);
    }
}