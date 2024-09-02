#include "Test.hpp"
#include "Algo.hpp"

namespace test
{
    void DrawCircle(sf::RenderWindow &window, const Shape &shape)
    {
        const pe2d::RigidObject body = shape.GetBody();
        const std::shared_ptr<pe2d::CircleCollider> collider = std::dynamic_pointer_cast<pe2d::CircleCollider>(body.GetCollider());

        const float radius = collider->GetRadius();
        const sf::Color color = shape.GetColor();

        sf::CircleShape circle(radius);
        circle.setFillColor(sf::Color::Transparent); 
        circle.setOutlineColor(color); 
        circle.setRotation(pe2dMath::RadiansToDeegres(circle.getRotation()));
        circle.setOutlineThickness(1.0f);

        const pe2d::Vector2 position = body.GetPosition();
        circle.setPosition(position.x - radius, position.y - radius);

        sf::VertexArray line(sf::Lines, 2);
        line[0].position = sf::Vector2f(position.x, position.y);
        line[0].color = color;
        line[1].position = sf::Vector2f(position.x + radius, position.y);
        line[1].color = color;
        window.draw(circle);
        window.draw(line);
    }

    void DrawBox(sf::RenderWindow &window, const Shape &shape)
    {
        const pe2d::RigidObject &body = shape.GetBody();
        const std::shared_ptr<pe2d::BoxCollider> collider = std::dynamic_pointer_cast<pe2d::BoxCollider>(body.GetCollider());
        const pe2d::Vector2 size = collider->GetSize() * body.GetScale();
        const sf::Color color = shape.GetColor();
        const pe2d::Vector2 pos = body.GetPosition();
        const float rotation = pe2dMath::RadiansToDeegres(body.GetRotation());
        
        sf::RectangleShape rec(sf::Vector2f(size.x, size.y));
        rec.setOrigin(rec.getSize() / 2.0f);
        rec.setPosition(sf::Vector2f(pos.x, pos.y));
        rec.setFillColor(sf::Color::Transparent);
        rec.setRotation(rotation);
        rec.setOutlineColor(color);
        rec.setOutlineThickness(1.0f);
        window.draw(rec);
        // DRAW BOUNDING BOX
        /*std::array<pe2d::Vector2, 4> vertices = body->GetBounadingBox();
        sf::Color color = shape.GetColor();
        for(int i = 0; i < vertices.size(); i++)
        {
            const int j = (i + 1) % vertices.size();
            sf::Vector2f vertex1 = sf::Vector2f{vertices[i].x , vertices[i].y};
            sf::Vector2f vertex2 = sf::Vector2f{vertices[j].x , vertices[j].y};
            sf::Vertex line[] =
            {
                sf::Vertex(vertex1, color),
                sf::Vertex(vertex2, color),
            };
            window.draw(line, 2, sf::Lines);
        }*/
    }

    void Test::AddCircle(const sf::Color color, pe2d::RigidObject &object)
    {
        m_World.AddObject(object);
        m_Shapes.emplace_back(color, m_World.At(object.GetID()), DrawCircle);
    }
    
    void Test::AddCircle(size_t ID, const sf::Color &color, float radius, pe2d::Transform transform, bool isStatic,
        float mass, pe2d::Vector2 velocity, pe2d::Vector2 gravity, float staticFriction, float dynamicFriction, float restitution)
    {
        std::shared_ptr<pe2d::CircleCollider> collider = std::make_shared<pe2d::CircleCollider>(radius);
        const float angluarVelocity = 0.0f;
        pe2d::RigidObject object = pe2d::RigidObject(ID, collider, transform, mass, velocity, angluarVelocity, gravity, isStatic,
                                                    staticFriction, dynamicFriction, restitution);
        m_World.AddObject(object);
        m_Shapes.emplace_back(color, m_World.At(ID), DrawCircle);
    }   
    
    void Test::AddBox(const sf::Color color, pe2d::RigidObject &object)
    {
        m_World.AddObject(object);
        m_Shapes.emplace_back(color, m_World.At(object.GetID()), DrawBox);
    }
    
    void Test::AddBox(unsigned int ID, const sf::Color &color, pe2d::Vector2 size, pe2d::Transform transform, bool isStatic,
        float mass, pe2d::Vector2 velocity, pe2d::Vector2 gravity, float staticFriction, float dynamicFriction, float restitution)
    {
        std::shared_ptr<pe2d::BoxCollider> collider = std::make_shared<pe2d::BoxCollider>(size);
        const float angluarVelocity = 0.0f;
        pe2d::RigidObject object = pe2d::RigidObject(ID, collider, transform, mass, velocity, angluarVelocity, gravity, isStatic,
                                                    staticFriction, dynamicFriction, restitution);
        m_World.AddObject(object);
        m_Shapes.emplace_back(color, m_World.At(ID), DrawBox);
    }

    void Test::Draw(sf::RenderWindow &window) const
    {
        for(int i = 0; i < m_Shapes.size(); i++)
        {
            m_Shapes[i].Draw(window, m_Shapes[i]);
        }
    }
}