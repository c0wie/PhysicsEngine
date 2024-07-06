#include "TestCollision.hpp"

namespace test
{
    TestCollision::TestCollision() :
        world()
    {
        std::shared_ptr<pe2d::Solver> solver = std::make_shared<pe2d::PositionSolver>();
        world.AddSolver(solver);
        auto object = CreateCollisionObject("Square", 100.0f, pe2d::Vector2{500.0f, 900.0f}, pe2d::Vector2{10.0f, 1.0f});
        world.AddCollisionObject(object);
        auto sqr = CreateCollisionObject("Square", 100.0f, pe2d::Vector2{500.0f, 500.0f}, pe2d::Vector2{1.0f, 1.0f}, 15.0f);
        world.AddCollisionObject(sqr);
    }

    void TestCollision::OnUpdate(float deltaTime, const sf::Vector2i &mousePos)
    {
        const pe2d::Vector2 s = world.GetObjects()[1]->GetPosition();
        const pe2d::Vector2 end = pe2d::Vector2{ (float)mousePos.x, (float)mousePos.y};
        const pe2d::Vector2 position = pe2d::Vector2::lerp(s, end, 16.0f * deltaTime);
        world.GetObjects()[1]->SetPosition(position);
        world.Step(deltaTime);
    }

    void TestCollision::OnRender(sf::RenderWindow &window)
    {
        for( const auto &object : world.GetObjects() )
        {
            Draw(window, object);
        }
    }

    void TestCollision::OnImGuiRender(sf::RenderWindow &window)
    {
        static bool showObjectEditor;
        static bool isRigidObject;
        static std::string name = "Square";
        static float size;
        static pe2d::Vector2 position;
        static pe2d::Vector2 scale = pe2d::Vector2{1.0f, 1.0f};
        static float rotation;
        static float mass;
        static pe2d::Vector2 velocity;
        static pe2d::Vector2 force;
        static bool takesGravity;
        static pe2d::Vector2 gravity;
        static float staticFriction;
        static float dynamicFriction;
        static float restitution;

        if( ImGui::Button("Add Object") )
        {
            showObjectEditor = true;
        }
        if( ImGui::Button("Clear Objects") )
        {
            ClearObjects();
        }
        ImGui::Text("Number of objects: %i", world.GetObjects().size());
        if(showObjectEditor)
        {
            ImGui::Begin("Object Editor");
            if(name == "Square")
            {
                if( ImGui::Button("Make Circle") )
                {
                    name = "Circle";
                }
            }
            else
            {
                if( ImGui::Button("Make Square") )
                {
                  name = "Square";
                }
            }
            ImGui::InputFloat2("Position", &position.x);
            ImGui::InputFloat("Size", &size);
            ImGui::InputFloat2("Scale", &scale.x);
            ImGui::InputFloat("Rotation", &rotation);
            if( ImGui::Button("CollisionObject") )
            {
                isRigidObject = false;
            }
            ImGui::SameLine();
            if( ImGui::Button("RigidObject") )
            {
                isRigidObject = true;
            }
            if(isRigidObject)
            {
                ImGui::InputFloat("Mass", &mass);
                ImGui::InputFloat2("Velocity", &velocity.x);
                ImGui::InputFloat2("Force", &force.x);
                if( ImGui::Button("Takes gravity from environment"))
                {
                    takesGravity = !takesGravity;
                }
                if(!takesGravity)
                {
                    ImGui::InputFloat2("Gravity", &gravity.x);
                }
                ImGui::InputFloat("Static Friction", &staticFriction);
                ImGui::InputFloat("Dynamic Friction", &dynamicFriction);
                ImGui::InputFloat("Restitution", &restitution);
            }
            if( ImGui::Button("Reset Variables") )
            {
                isRigidObject = false;
                name = "Square";
                position = pe2d::Vector2{};
                size = 0.0f;
                scale = pe2d::Vector2{1.0f, 1.0f};
                rotation = 0.0f;
                mass = 0.0f;
                velocity = pe2d::Vector2{};
                force = pe2d::Vector2{};
                takesGravity = false;
                gravity = pe2d::Vector2{};
                staticFriction = 0.0f;
                dynamicFriction = 0.0f;
                restitution = 0.0f;
            }
            
            if( ImGui::Button("Create") )
            {
                if(isRigidObject)
                {
                    world.AddRigidObject( CreateRigidObject(name, size, mass, position, velocity, scale, rotation, false, gravity,
                    takesGravity, staticFriction, dynamicFriction, restitution) );
                }
                else
                {
                    world.AddCollisionObject( CreateCollisionObject(name, size, position, scale, rotation, false) );
                }
                showObjectEditor = false;
            }
            ImGui::End();
        }
    }

    std::shared_ptr<pe2d::CollisionObject> TestCollision::CreateCollisionObject(const std::string &type, float size, const pe2d::Vector2 &position,
            const pe2d::Vector2 &scale, float rotation, bool trigger)
    {
        const pe2d::Transform transform = pe2d::Transform{ position, scale, rotation };
        if(type == "Circle")
        {
            auto collider = std::make_shared<pe2d::CircleCollider>(size);
            std::shared_ptr<pe2d::CollisionObject> object = std::make_shared<pe2d::CollisionObject>(collider, transform, trigger, nullptr);
            return object;
        }
        else if(type == "Square")
        {
            auto collider = std::make_shared<pe2d::SquareCollider>(size);
            std::shared_ptr<pe2d::CollisionObject> object = std::make_shared<pe2d::CollisionObject>(collider, transform, trigger, nullptr);
            return object;
        }
        ASSERT("NOT VALID TYPE OF OBJECT");
    }

    std::shared_ptr<pe2d::RigidObject> TestCollision::CreateRigidObject(const std::string &type, float size, float mass, const pe2d::Vector2 &position,
            const pe2d::Vector2 &velocity, const pe2d::Vector2 &scale, float rotation, bool trigger, const pe2d::Vector2 &gravity,
            bool takesGravity, float staticFriction, float dynamicFriction, float restitution)
    {
        const pe2d::Transform transform = pe2d::Transform{position, scale, rotation};
        if(type == "Circle")
        {
            auto collider = std::make_shared<pe2d::CircleCollider>(size);
            auto object = std::make_shared<pe2d::RigidObject>(collider, transform, trigger, nullptr, mass, velocity, pe2d::Vector2{0.0f, 0.0f},
                gravity, takesGravity, staticFriction, dynamicFriction, restitution);
            return object;
        }
        else if(type == "Square")
        {
            auto collider = std::make_shared<pe2d::SquareCollider>(size);
            auto object = std::make_shared<pe2d::RigidObject>(collider, transform, trigger, nullptr, mass, velocity, pe2d::Vector2{0.0f, 0.0f},
                gravity, takesGravity, staticFriction, dynamicFriction, restitution);
            return object;
        }
        ASSERT("NOT VALID TYPE OF OBJECT");
    }

    void TestCollision::ClearObjects()
    {
        auto objects = world.GetObjects();
        for(int i = 2; i < objects.size(); i++)
        {
            world.RemoveObject(objects[i]);
        }
    }

    void TestCollision::Draw(sf::RenderWindow &window, const std::shared_ptr<pe2d::CollisionObject> obj) const
    {
        const sf::Vector2f position = sf::Vector2f{obj->GetTransform().position.x, obj->GetTransform().position.y};
        std::shared_ptr<pe2d::CircleCollider> circleCollider = std::dynamic_pointer_cast<pe2d::CircleCollider>(obj->GetCollider());

        if(circleCollider != nullptr)
        {
            float radius = circleCollider->GetRadius() * obj->GetTransform().scale.x;
            sf::CircleShape circle(circleCollider->GetRadius());
            circle.setOrigin(circle.getRadius(), circle.getRadius());
            circle.setPosition(position);
            circle.setFillColor(sf::Color::Cyan);
            window.draw(circle);
        }
        else
        {
            std::shared_ptr<pe2d::SquareCollider> squareCollider = std::dynamic_pointer_cast<pe2d::SquareCollider>(obj->GetCollider());
            const sf::Vector2f size{ squareCollider->GetSideLength() * obj->GetTransform().scale.x,
                                     squareCollider->GetSideLength() * obj->GetTransform().scale.y };
            sf::RectangleShape square(size);
            square.setOrigin(square.getSize() / 2.0f);
            square.setPosition(position);
            square.setRotation(obj->GetTransform().rotation);
            square.setFillColor(sf::Color::Red);
            window.draw(square);
        }
    }
}   