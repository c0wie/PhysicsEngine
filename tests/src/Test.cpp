#include "Test.hpp"

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
        const pe2d::Vector2 position = body->GetTransform().position;
        const pe2d::Vector2 scale = body->GetTransform().scale;
        const sf::Color color = shape.GetColor();
        
        sf::RectangleShape box(sf::Vector2f{size.x, size.y});
        box.setOrigin(box.getSize() / 2.0f);
        box.setPosition(position.x, position.y);
        box.setScale(scale.x, scale.y);
        box.setRotation(body->GetTransform().rotation);
        box.setFillColor(color);
        window.draw(box);
    }

    void Test::DrawConvexShape(sf::RenderWindow &window, const Shape &shape)
    {
        const std::shared_ptr<pe2d::CollisionObject> body = shape.GetBody();
        const std::shared_ptr<pe2d::ConvexShapeCollider> collider = std::dynamic_pointer_cast<pe2d::ConvexShapeCollider>(body->GetCollider());
        const unsigned int verteciesCount = collider->GetVerteciesCount();
        const pe2d::Vector2 position = body->GetTransform().position;
        const pe2d::Vector2 scale = body->GetTransform().scale;
        const pe2d::Vector2 *vertecies = collider->GetVertecies();
        const sf::Color color = shape.GetColor();

        sf::ConvexShape convexShape(verteciesCount);
        for(int i = 0; i < verteciesCount; i++)
        {
            convexShape.setPoint(i, sf::Vector2f{vertecies[i].x, vertecies[i].y });
        }
        convexShape.setOrigin(position.x, position.y);
        convexShape.setScale(scale.x, scale.y);
        convexShape.setRotation(body->GetTransform().rotation);
        convexShape.setFillColor(color);
        window.draw(convexShape);
    }

    void Test::AddCircle(const sf::Color &color, float radius, const pe2d::Transform &transform, bool isTrigger)
    {
        const std::shared_ptr<pe2d::CircleCollider> collider = std::make_shared<pe2d::CircleCollider>(radius);
        Shape shape = Shape(color, collider ,transform, isTrigger, DrawCircle);
        m_Shapes.push_back(shape);
        world.AddCollisionObject(shape.GetBody());
    }

    void Test::AddCircle(const sf::Color &color, float radius, const pe2d::Transform &transform, bool isTrigger, float mass, const pe2d::Vector2 &velocity, const pe2d::Vector2 &gravity)
    {
        const std::shared_ptr<pe2d::CircleCollider> collider = std::make_shared<pe2d::CircleCollider>(radius);
        Shape shape = Shape(color, collider ,transform, isTrigger, mass, velocity, pe2d::Vector2{}, gravity, false, DrawCircle);
        m_Shapes.push_back(shape);
        world.AddCollisionObject(shape.GetBody());
    }

    void Test::AddBox(const sf::Color &color, const pe2d::Vector2 &size, const pe2d::Transform &transform, bool isTrigger)
    {
        const std::shared_ptr<pe2d::BoxCollider> collider = std::make_shared<pe2d::BoxCollider>(size);
        Shape shape = Shape(color, collider ,transform, isTrigger, DrawBox);
        m_Shapes.push_back(shape);
        world.AddCollisionObject(shape.GetBody());
    }

    void Test::AddBox(const sf::Color &color, const pe2d::Vector2 &size, const pe2d::Transform &transform, bool isTrigger, float mass, const pe2d::Vector2 &velocity, const pe2d::Vector2 &gravity)
    {
        const std::shared_ptr<pe2d::BoxCollider> collider = std::make_shared<pe2d::BoxCollider>(size);
        Shape shape = Shape(color, collider ,transform, isTrigger, mass, velocity, pe2d::Vector2{}, gravity, false, DrawBox);
        m_Shapes.push_back(shape);
        world.AddCollisionObject(shape.GetBody());
    }

    void Test::AddConvexShape(const sf::Color &color, unsigned int verteciesCount, pe2d::Vector2 *vertecies, const pe2d::Transform &transform, bool isTrigger)
    {
        const std::shared_ptr<pe2d::ConvexShapeCollider> collider = std::make_shared<pe2d::ConvexShapeCollider>(vertecies, verteciesCount, transform.position);
        Shape shape = Shape(color, collider ,transform, isTrigger, DrawConvexShape);
        m_Shapes.push_back(shape);
        world.AddCollisionObject(shape.GetBody());
    }
    
    void Test::AddConvexShape(const sf::Color &color, unsigned int verteciesCount, pe2d::Vector2 *vertecies, const pe2d::Transform &transform,
            bool isTrigger, float mass, const pe2d::Vector2 &velocity, const pe2d::Vector2 &gravity)
    {
        const std::shared_ptr<pe2d::ConvexShapeCollider> collider = std::make_shared<pe2d::ConvexShapeCollider>(vertecies, verteciesCount, transform.position);
        Shape shape = Shape(color, collider ,transform, isTrigger, mass, velocity, pe2d::Vector2{}, gravity, false, DrawConvexShape);
        m_Shapes.push_back(shape);
        world.AddCollisionObject(shape.GetBody());
    }

    void Test::Draw(sf::RenderWindow &window) const
    {
        for(int i = 0; i < m_Shapes.size(); i++)
        {
            m_Shapes[i].Draw(window, m_Shapes[i]);
        }
    }

    void Test::ClearObjects()
    {
        m_Shapes.clear();
        auto objects = world.GetObjects();
        for(int i = 0; i < objects.size(); i++)
        {
            world.RemoveObject(objects[i]);
        }
    }
}