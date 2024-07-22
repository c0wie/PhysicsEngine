#pragma once

#include <SFML/Graphics.hpp>
#include "../../pe2d/include/RigidObject.hpp"

namespace test
{
    class Shape
    {
    public:
        Shape(const sf::Color &color, std::shared_ptr<pe2d::CollisionObject> body, void (*drawFunction)(sf::RenderWindow &window, const Shape &shape)) : 
        m_Color(color),
        m_Body(body),
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
