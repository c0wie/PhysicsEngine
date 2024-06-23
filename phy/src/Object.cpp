#include "../headers/Object.hpp"

namespace phy
{
    Object::Object() :
        Mass(0.0f), Velocity(0.0f, 0.0f), Force(0.0f, 0.0f), Collider(nullptr), Transform(nullptr) 
    {
        LogCall("Default Object Constructor called");
    };

    Object::Object(float mass, const Vector2 &velocity, const Vector2 &force, 
        const std::shared_ptr<phy::SquareCollider> &collider, const std::shared_ptr<phy::Transform> &transform) :
        Mass(mass), Velocity(velocity), Force(force), Collider(collider), Transform(transform) 
    {
        LogCall("Params Object Constructor called");
    };

    Object::Object(float mass, const Vector2 &velocity, const Vector2 &force, 
        const std::shared_ptr<phy::CircleCollider> &collider, const std::shared_ptr<phy::Transform> &transform) :
        Mass(mass), Velocity(velocity), Force(force), Collider(collider), Transform(transform)
    {
        LogCall("Params Object Constructor called");
    }

    Object::Object(const Object &other) :
        Mass(other.Mass), Velocity(other.Velocity), Force(other.Force), Collider(other.Collider), Transform(other.Transform) 
    {
        LogCall("Object Copy Constructor called");
    };

    Object::Object (Object &&other) noexcept :
        Mass(other.Mass), Velocity(other.Velocity), Force(other.Force), Collider(other.Collider), Transform(other.Transform)
    {
        other.Mass = 0.0f;
        other.Velocity = Vector2(0.0f, 0.0f);
        other.Force = Vector2(0.0f, 0.0f);
        Collider = nullptr;
        Transform = nullptr;
        LogCall("Object Move Constructor called");
    };

    Object& Object::operator=(const Object &other)
    {
        if(this == &other)
        {
            return *this;
        }
        Mass = other.Mass;
        Velocity = other.Velocity;
        Force = other.Force;
        Collider = other.Collider;
        Transform = other.Transform;
        LogCall("Copy Assigment Operator Called");
        return *this;
    }

    Object& Object::operator=(Object &&other) noexcept
    {
        if(this == &other)
        {
            return *this;
        }
        Mass = other.Mass;
        Velocity = other.Velocity;
        Force = other.Force;
        Collider = other.Collider;
        Transform = other.Transform;

        other.Mass = 0.0f;
        other.Velocity = Vector2(0.0f, 0.0f);
        other.Force = Vector2(0.0f, 0.0f);
        Collider = nullptr;
        Transform = nullptr;
        LogCall("Move Assigment Operator Called");
        return *this;
    }
}