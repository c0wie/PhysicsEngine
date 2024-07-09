#pragma once

#include <SFML/Graphics.hpp>
#include "../../pe2d/include/RigidObject.hpp"

namespace test
{
    class Shape
    {
    public:
        Shape(const sf::Color &color, const std::shared_ptr<pe2d::CircleCollider> collider, const pe2d::Transform &transform, bool isTrigger,
        void (*drawFunction)(sf::RenderWindow &window, const Shape &shape)) : 
        m_Color(color),
        m_Body( std::make_shared<pe2d::CollisionObject>(collider, transform, isTrigger, nullptr) ),
        m_Draw(drawFunction)
    {}
        Shape(const sf::Color &color, const std::shared_ptr<pe2d::CircleCollider> collider, const pe2d::Transform &transform, bool isTrigger,
        float mass, const pe2d::Vector2 &velocity, const pe2d::Vector2 &force, const pe2d::Vector2 &gravity, bool takesGravity,  void (*drawFunction)(sf::RenderWindow &window, const Shape &shape)) :
        m_Color(color),
        m_Body( std::make_shared<pe2d::RigidObject>(collider, transform, isTrigger, nullptr, mass, velocity, force, gravity, takesGravity, 0.0f, 0.0f, 0.0f)),
        m_Draw(drawFunction)
    {}
        Shape(const sf::Color &color, const std::shared_ptr<pe2d::BoxCollider> collider, const pe2d::Transform &transform, bool isTrigger,  void (*drawFunction)(sf::RenderWindow &window, const Shape &shape)) : 
        m_Color(color),
        m_Body( std::make_shared<pe2d::CollisionObject>(collider, transform, isTrigger, nullptr) ),
        m_Draw(drawFunction)
    {}
        Shape(const sf::Color &color, const std::shared_ptr<pe2d::BoxCollider> collider, const pe2d::Transform &transform, bool isTrigger,
        float mass, const pe2d::Vector2 &velocity, const pe2d::Vector2 &force, const pe2d::Vector2 &gravity, bool takesGravity,  void (*drawFunction)(sf::RenderWindow &window, const Shape &shape)) :
        m_Color(color),
        m_Body( std::make_shared<pe2d::RigidObject>(collider, transform, isTrigger, nullptr, mass, velocity, force, gravity, takesGravity, 0.0f, 0.0f, 0.0f) ),
        m_Draw(drawFunction)
    {}
        Shape(const sf::Color &color, const std::shared_ptr<pe2d::ConvexShapeCollider> collider, const pe2d::Transform &transform, bool isTrigger,  void (*drawFunction)(sf::RenderWindow &window, const Shape &shape)) : 
        m_Color(color),
        m_Body( std::make_shared<pe2d::CollisionObject>(collider, transform, isTrigger, nullptr) ),
        m_Draw(drawFunction)
    {}
    Shape(const sf::Color &color, const std::shared_ptr<pe2d::ConvexShapeCollider> collider, const pe2d::Transform &transform, bool isTrigger,
    float mass, const pe2d::Vector2 &velocity, const pe2d::Vector2 &force, const pe2d::Vector2 &gravity, bool takesGravity,  void (*drawFunction)(sf::RenderWindow &window, const Shape &shape)) :
        m_Color(color),
        m_Body( std::make_shared<pe2d::RigidObject>(collider, transform, isTrigger, nullptr, mass, velocity, force, gravity, takesGravity, 0.0f, 0.0f, 0.0f) ),
        m_Draw(drawFunction)
    {}
    public:
        sf::Color GetColor() const { return m_Color; }
        std::shared_ptr<pe2d::CollisionObject> GetBody() const { return m_Body; }
        void SetColor(sf::Color color) { m_Color = color; }
        void Draw(sf::RenderWindow &window, const Shape &shape) const
        {
            m_Draw(window, shape);     
        }
    protected:
        std::shared_ptr<pe2d::CollisionObject> m_Body;
        sf::Color m_Color;
        void (*m_Draw)(sf::RenderWindow &, const Shape &);
    };
}
