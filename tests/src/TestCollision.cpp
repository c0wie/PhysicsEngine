#include "TestCollision.hpp"

namespace test
{
    TestCollision::TestCollision() :
        world(),
        showObjectEditor(false)
    {
        world.AddCollisionObject(CreateCollisionObject("Circle", 40.0f, pe2d::Vector2{400.0f, 200.0f}));
    }

    void TestCollision::OnUpdate(float deltaTime)
    {
        world.Step(deltaTime);
    }

    void TestCollision::OnRender(sf::RenderWindow &window)
    {
        for( const auto &object : world.GetObjects() )
        {
            Draw(window, object);
        }
    }

    void TestCollision::OnImGuiRender(sf::RenderWindow &window, const sf::Vector2i &mousePosition)
    {
        if(ImGui::Button("Add Object"))
        {
            showObjectEditor = true;
        }
        if(showObjectEditor)
        {
            ImGui::Begin("Object Editor");
            if(ImGui::Button("Go Back"))
            {
                showObjectEditor = false;
            }
            ImGui::End();
        }
    }

    std::shared_ptr<pe2d::CollisionObject> TestCollision::CreateCollisionObject(const std::string &type, float size, const pe2d::Vector2 &position,
            const pe2d::Vector2 &scale, float rotation, bool trigger)
    {
        std::shared_ptr<pe2d::Transform> transform = std::make_shared<pe2d::Transform>(position, scale, rotation);
        
        if(type == "Circle")
        {
            auto collider = std::make_shared<pe2d::CircleCollider>(size);
            std::shared_ptr<pe2d::CollisionObject> object = std::make_shared<pe2d::CollisionObject>(collider, transform, trigger);
            return object;
        }
        else if(type == "Square")
        {
            auto collider = std::make_shared<pe2d::SquareCollider>(size);
            std::shared_ptr<pe2d::CollisionObject> object = std::make_shared<pe2d::CollisionObject>(collider, transform, trigger);
            return object;
        }
        ASSERT("NOT VALID TYPE OF OBJECT");
    }

    std::shared_ptr<pe2d::RigidObject> TestCollision::CreateRigidObject(const std::string &type, float size, float mass, const pe2d::Vector2 &position,
            const pe2d::Vector2 &velocity, const pe2d::Vector2 &scale, float rotation, bool trigger, const pe2d::Vector2 &gravity,
            bool takesGravity, float staticFriction, float dynamicFriction, float restitution)
    {
        std::shared_ptr<pe2d::Transform> transform = std::make_shared<pe2d::Transform>(position, scale, rotation);
        if(type == "Circle")
        {
            auto collider = std::make_shared<pe2d::CircleCollider>(size);
            auto object = std::make_shared<pe2d::RigidObject>(collider, transform, trigger, mass, velocity, pe2d::Vector2{0.0f, 0.0f},
                gravity, takesGravity, staticFriction, dynamicFriction, restitution);
        }
        else if(type == "Square")
        {
            auto collider = std::make_shared<pe2d::SquareCollider>(size);
            auto object = std::make_shared<pe2d::RigidObject>(collider, transform, trigger, mass, velocity, pe2d::Vector2{0.0f, 0.0f},
                gravity, takesGravity, staticFriction, dynamicFriction, restitution);
        }
        ASSERT("NOT VALID TYPE OF OBJECT");
    }

    void TestCollision::Draw(sf::RenderWindow &window, const std::shared_ptr<pe2d::CollisionObject> obj) const
    {
        const sf::Vector2f position = sf::Vector2f{obj->GetTransform()->position.x, obj->GetTransform()->position.y};
        std::shared_ptr<pe2d::CircleCollider> circleCollider = std::dynamic_pointer_cast<pe2d::CircleCollider>(obj->GetCollider());

        if(circleCollider != nullptr)
        {
            sf::CircleShape circle(circleCollider->GetRadius());
            circle.setOrigin(circle.getRadius(), circle.getRadius());
            circle.setPosition(position);
            circle.setFillColor(sf::Color::Cyan);
            window.draw(circle);
        }
        else
        {
            std::shared_ptr<pe2d::SquareCollider> squareCollider = std::dynamic_pointer_cast<pe2d::SquareCollider>(obj->GetCollider());
            const sf::Vector2f size{squareCollider->GetSideLength(), squareCollider->GetSideLength()};
            sf::RectangleShape square(size);
            square.setOrigin(square.getSize() / 2.0f);
            square.setPosition(position);
            square.setFillColor(sf::Color::Red);
            window.draw(square);
        }
    }
}   