#include "Test.hpp"
#include "../../pe2d/include/Algo.hpp"

namespace test
{
    void Test::DrawCircle(sf::RenderWindow &window, const Shape &shape)
    {
        const std::shared_ptr<pe2d::CollisionObject> body = shape.GetBody();
        const std::shared_ptr<pe2d::CircleCollider> collider = std::dynamic_pointer_cast<pe2d::CircleCollider>(body->GetCollider());
        const float radius = collider->GetRadius();
        const pe2d::Vector2 position = body->GetTransform().position;
        const pe2d::Vector2 scale = body->GetTransform().scale;
        const sf::Color color = shape.GetColor();
        
        sf::CircleShape circle(radius);
        circle.setOrigin(radius, radius);
        circle.setPosition(position.x, position.y);
        circle.setScale(scale.x, scale.y);
        circle.setRotation(body->GetTransform().rotation);
        circle.setFillColor(color);
        window.draw(circle);
    }

    void Test::DrawBox(sf::RenderWindow &window, const Shape &shape)
    {
        const std::shared_ptr<pe2d::CollisionObject> body = shape.GetBody();
        const std::shared_ptr<pe2d::BoxCollider> collider = std::dynamic_pointer_cast<pe2d::BoxCollider>(body->GetCollider());
        const pe2d::Vector2 size = collider->GetSize();
        const sf::Color color = shape.GetColor();
        
        std::vector<pe2d::Vector2> vertices = pe2d::algo::GetBoxVertices(size, body->GetTransform());
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
        }
    }

    void Test::AddCircle(unsigned int ID, const sf::Color &color, float radius, const pe2d::Transform &transform, bool isTrigger)
    {
        const std::shared_ptr<pe2d::CircleCollider> collider = std::make_shared<pe2d::CircleCollider>(radius);
        const std::shared_ptr<pe2d::CollisionObject> body = std::make_shared<pe2d::CollisionObject>(ID, collider ,transform, isTrigger, nullptr);
        Shape shape = Shape(color, body, DrawCircle);
        m_Shapes.push_back(shape);
        m_World.AddCollisionObject(shape.GetBody());
    }

    void Test::AddCircle(unsigned int ID, const sf::Color &color, float radius, const pe2d::Transform &transform, bool isTrigger, float mass, pe2d::Vector2 velocity, pe2d::Vector2 gravity)
    {
        const std::shared_ptr<pe2d::CircleCollider> collider = std::make_shared<pe2d::CircleCollider>(radius);
        const std::shared_ptr<pe2d::CollisionObject> body = std::make_shared<pe2d::RigidObject>(ID, collider ,transform, isTrigger, nullptr, mass, velocity,
                                                            pe2d::Vector2{}, gravity, false, 0.0f, 0.0f, 0.0f);
        Shape shape = Shape(color, body, DrawCircle);
        m_Shapes.push_back(shape);
        m_World.AddCollisionObject(shape.GetBody());
    }

    void Test::AddBox(unsigned int ID, const sf::Color &color, pe2d::Vector2 size, const pe2d::Transform &transform, bool isTrigger)
    {
        std::shared_ptr<pe2d::BoxCollider> collider = std::make_shared<pe2d::BoxCollider>(size);
        std::shared_ptr<pe2d::CollisionObject> body = std::make_shared<pe2d::CollisionObject>(ID, collider, transform, isTrigger, nullptr);
        Shape shape = Shape(color, body, DrawBox);
        m_Shapes.push_back(shape);
        m_World.AddCollisionObject(body);
    }

    void Test::AddBox(unsigned int ID, const sf::Color &color, pe2d::Vector2 size, const pe2d::Transform &transform, bool isTrigger, float mass, pe2d::Vector2 velocity, pe2d::Vector2 gravity)
    {
        const std::shared_ptr<pe2d::BoxCollider> collider = std::make_shared<pe2d::BoxCollider>(size);
        const std::shared_ptr<pe2d::CollisionObject> body = std::make_shared<pe2d::RigidObject>(ID, collider ,transform, isTrigger, nullptr, mass, velocity,
                                                            pe2d::Vector2{}, gravity, false, 0.0f, 0.0f, 0.0f);
        Shape shape = Shape(color, body, DrawBox);
        m_Shapes.push_back(shape);
        m_World.AddCollisionObject(shape.GetBody());
    }

    void Test::Draw(sf::RenderWindow &window) const
    {
        for(int i = 0; i < m_Shapes.size(); i++)
        {
            const pe2d::Vector2 position = m_Shapes[i].GetBody()->GetPosition();
            if(position.y > 1100.0f || position.y < -100.0f || position.x < -100.0f || position.x > 1100.0f)
            {
                continue;
            }
            m_Shapes[i].Draw(window, m_Shapes[i]);
        }
    }
}