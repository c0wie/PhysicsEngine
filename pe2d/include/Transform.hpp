#pragma once

#include "Vector2.hpp"

namespace pe2d
{
    struct Transform
    {
    public:
        constexpr Transform() : 
            position(Vector2(0.0f, 0.0f)), scale(Vector2(0.0f, 0.0f)), rotation(0.0f) {}
        constexpr Transform(const Vector2 &position, const Vector2 &scale, float rotation) : 
            position(position), scale(scale), rotation(rotation) {}
        constexpr Transform(const Transform &other) = default;
        constexpr Transform(Transform &&other) noexcept = default;
        constexpr Transform& operator=(const Transform &other) = default;
        constexpr Transform& operator=(Transform &&other) noexcept = default;
    public:
        constexpr void Move(const Vector2 &offset) { position += offset; }
        constexpr void Rotate(float angle) { rotation += angle; }
        constexpr float GetRotationInRadians() const { return rotation * (PI / 180.0f); }
    public:
        //center of object
        Vector2 position;
        Vector2 scale;
        // in deegres
        float rotation;
    };
}