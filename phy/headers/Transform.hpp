#pragma once

#include "Vector2.hpp"

namespace phy
{
    struct Transform
    {
    public:
        Vector2 position;
        Vector2 scale;
        Vector2 rotation;
    public:
        constexpr Transform() :
            position(Vector2{0.0f, 0.0f}), scale(Vector2{0.0f, 0.0f}), rotation(Vector2{0.0f, 0.0f}) {}
        constexpr Transform(const Vector2 &position, const Vector2 &scale, const Vector2 &rotation) :
            position(position), scale(scale), rotation(rotation) {}
        constexpr Transform(const Transform &other) = default;
        constexpr Transform(Transform &&other) noexcept :
            position(other.position), scale(other.scale), rotation(other.rotation)
        {
            other.position = Vector2{};
            other.scale = Vector2{};
            other.rotation = Vector2{};
        }
        constexpr Transform& operator=(const Transform &other) = default;
        constexpr Transform& operator=(Transform &&other) noexcept
        {
            if(this == &other)
            {
                return *this;
            }
            position = other.position;
            scale = other.scale;
            rotation = other.rotation;
            other.position = Vector2{};
            other.scale = Vector2{};
            other.rotation = Vector2{};
            return *this;
        }

        constexpr void Move(const Vector2 &offset)
        {
            position.x += offset.x;
            position.y += offset.y;
        }
        //not implemented yet
        constexpr void Rotate(float angle)
        {
            
        }
    };
}