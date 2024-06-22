#include "../headers/Transform.hpp"

namespace phy
{
    Transform::Transform() :
        Position(Vector2(0.0f, 0.0f)), Scale(Vector2(0.0f, 0.0f)) {};

    Transform::Transform(const Vector2 &position, const Vector2 &scale) :
        Position(position), Scale(scale) {};

    Transform::Transform(const Transform &other) :
        Position(other.Position), Scale(other.Scale) {};

    Transform::Transform(Transform &&other) noexcept :
        Position(other.Position), Scale(other.Scale)
    {
        other.Position = Vector2(0.0f, 0.0f);
        other.Scale = Vector2(0.0f, 0.0f);
    };

    Transform& Transform::operator=(const Transform &other)
    {
        if(this == &other)
        {
            return *this;
        }
        Position = other.Position;
        Scale = other.Scale;
        return *this;
    }

    Transform& Transform::operator=(Transform &&other) noexcept
    {
        if(this == &other)
        {
            return *this;
        }
        Position = other.Position;
        Scale = other.Scale;
        other.Position = Vector2(0.0f, 0.0f);
        other.Scale = Vector2(0.0f, 0.0f);
        return *this;
    }
}