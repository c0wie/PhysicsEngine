#include "Test.hpp"
#include "../../pe2d/include/Algo.hpp"

namespace test
{
    void Test::DrawCircle(sf::RenderWindow &window, const Shape &shape)
    {
        const std::shared_ptr<pe2d::CollisionObject> body = shape.GetBody();
        const std::shared_ptr<pe2d::CircleCollider> collider = std::dynamic_pointer_cast<pe2d::CircleCollider>(body->GetCollider());

        const float radius = collider->GetRadius();
        const sf::Color color = shape.GetColor();

        sf::CircleShape circle(radius);
        circle.setFillColor(sf::Color::Transparent); 
        circle.setOutlineColor(color); 
        circle.setOutlineThickness(1.0f);

        const pe2d::Vector2 position = body->GetPosition();
        circle.setPosition(position.x - radius, position.y - radius);

        window.draw(circle);
    }

    void Test::DrawBox(sf::RenderWindow &window, const Shape &shape)
    {
        const std::shared_ptr<pe2d::CollisionObject> body = shape.GetBody();
        const std::shared_ptr<pe2d::BoxCollider> collider = std::dynamic_pointer_cast<pe2d::BoxCollider>(body->GetCollider());
        const pe2d::Vector2 size = collider->GetSize() * body->GetScale();
        const sf::Color color = shape.GetColor();
        const pe2d::Vector2 pos = body->GetPosition();
        
        sf::RectangleShape rec(sf::Vector2f(size.x, size.y));
        rec.setOrigin(rec.getSize() / 2.0f);
        rec.setPosition(sf::Vector2f(pos.x, pos.y));
        rec.setFillColor(sf::Color::Transparent);
        rec.setRotation(body->GetRotation());
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

    void Test::AddCircle(unsigned int ID, const sf::Color &color, float radius, pe2d::Transform transform, bool isTrigger)
    {
        const std::shared_ptr<pe2d::CircleCollider> collider = std::make_shared<pe2d::CircleCollider>(radius);
        const std::shared_ptr<pe2d::CollisionObject> body = std::make_shared<pe2d::CollisionObject>(ID, collider ,transform, isTrigger, nullptr);
        Shape shape = Shape(color, body, DrawCircle);
        m_Shapes.push_back(shape);
        m_World.AddCollisionObject(body);
    }

    void Test::AddCircle(unsigned int ID, const sf::Color &color, float radius, pe2d::Transform transform, bool isTrigger, float mass, pe2d::Vector2 velocity, pe2d::Vector2 gravity)
    {
        const std::shared_ptr<pe2d::CircleCollider> collider = std::make_shared<pe2d::CircleCollider>(radius);
        const std::shared_ptr<pe2d::RigidObject> body = std::make_shared<pe2d::RigidObject>(ID, collider ,transform, isTrigger, nullptr, mass, velocity,
                                                            pe2d::Vector2(0.0f, 0.0f), gravity, false, 0.0f, 0.0f, 0.0f);
        Shape shape = Shape(color, body, DrawCircle);
        m_Shapes.push_back(shape);
        m_World.AddRigidObject(body);
    }

    void Test::AddBox(unsigned int ID, const sf::Color &color, pe2d::Vector2 size, pe2d::Transform transform, bool isTrigger)
    {
        std::shared_ptr<pe2d::BoxCollider> collider = std::make_shared<pe2d::BoxCollider>(size);
        std::shared_ptr<pe2d::CollisionObject> body = std::make_shared<pe2d::CollisionObject>(ID, collider, transform, isTrigger, nullptr);
        Shape shape = Shape(color, body, DrawBox);
        m_Shapes.push_back(shape);
        m_World.AddCollisionObject(body);
    }

    void Test::AddBox(unsigned int ID, const sf::Color &color, pe2d::Vector2 size, pe2d::Transform transform, bool isTrigger, float mass, pe2d::Vector2 velocity, pe2d::Vector2 gravity)
    {
        const std::shared_ptr<pe2d::BoxCollider> collider = std::make_shared<pe2d::BoxCollider>(size);
        const std::shared_ptr<pe2d::RigidObject> body = std::make_shared<pe2d::RigidObject>(ID, collider ,transform, isTrigger, nullptr, mass, velocity,
                                                            pe2d::Vector2{}, gravity, false, 0.0f, 0.0f, 0.0f);
        Shape shape = Shape(color, body, DrawBox);
        m_Shapes.push_back(shape);
        m_World.AddRigidObject(body);
    }

    void Test::Draw(sf::RenderWindow &window) const
    {
        for(int i = 0; i < m_Shapes.size(); i++)
        {
            m_Shapes[i].Draw(window, m_Shapes[i]);
        }
    }
}