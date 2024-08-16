#pragma once

#include <SFML/Graphics.hpp>
#include "../../pe2d/include/RigidObject.hpp"

namespace test
{
    struct Shape
    {
    public:
        Shape(const sf::Color &color, pe2d::RigidObject &body, void (*drawFunction)(sf::RenderWindow &window, const Shape &shape)) : 
        m_Color(color),
        m_Body(body),
        m_Draw(drawFunction)
    {}
    public:
        sf::Color GetColor() const { return m_Color; }
        pe2d::RigidObject GetBody() const { return m_Body; }
        void SetColor(sf::Color color) { m_Color = color; }
        void Draw(sf::RenderWindow &window, const Shape &shape) const
        {
            m_Draw(window, shape);     
        }
    protected:
        pe2d::RigidObject &m_Body;
        sf::Color m_Color;
        void (*m_Draw)(sf::RenderWindow &, const Shape &);
    };
}
