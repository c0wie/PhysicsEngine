#pragma once

#include <memory>
#include "Vector2.hpp"
#include "CollisionPoints.hpp"
#include "Transform.hpp"
#include "SphereCollider.hpp"
#include "PlaneCollider.hpp"

namespace phy
{
    class Object
    {
    public:
        float Mass;
        Vector2 Velocity;
        Vector2 Force;
        std::shared_ptr<SphereCollider> Collider;
        std::shared_ptr<Transform> Transform;
    public:
        Object() :
            Mass(0.0f), Velocity(0.0f, 0.0f), Force(0.0f, 0.0f), Collider(nullptr), Transform(nullptr) 
        {
            LogCall("Default Object Constructor called");
        };

        Object(float mass, const Vector2 &velocity, const Vector2 &force, 
            const std::shared_ptr<phy::SphereCollider> &collider, const std::shared_ptr<phy::Transform> &transform) :
            Mass(mass), Velocity(velocity), Force(force), Collider(collider), Transform(transform) 
        {
            LogCall("Params Object Constructor called");
        };

        Object (const Object &other) :
            Mass(other.Mass), Velocity(other.Velocity), Force(other.Force), Collider(other.Collider), Transform(other.Transform) 
        {
            LogCall("Object Copy Constructor called");
        };

        Object (Object &&other) :
            Mass(other.Mass), Velocity(other.Velocity), Force(other.Force), Collider(other.Collider), Transform(other.Transform)
        {
            other.Mass = 0.0f;
            other.Velocity = Vector2(0.0f, 0.0f);
            other.Force = Vector2(0.0f, 0.0f);
            Collider = nullptr;
            Transform = nullptr;
            LogCall("Object Move Constructor called");
        };

        Object& operator=(const Object &other)
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

        Object& operator=(Object &&other)
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
    };
}