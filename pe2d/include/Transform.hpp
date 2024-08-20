#pragma once

#include "Vector2.hpp"

namespace pe2d
{
    /* 
        Struct holding basic information about object like:
        - position
        - scale
        - rotation in degrees
    */
    struct Transform
    {
    public:
        constexpr Transform() : 
            position(Vector2(0.0f, 0.0f)), scale(Vector2(0.0f, 0.0f)), rotation(0.0f) {}
        constexpr Transform(Vector2 position, Vector2 scale, float rotation) : 
            position(position), scale(scale), rotation(rotation) {}
        constexpr Transform(const Transform &other) = default;
        constexpr Transform(Transform &&other) noexcept = default;
        constexpr Transform& operator=(const Transform &other) = default;
        constexpr Transform& operator=(Transform &&other) noexcept = default;
    public:
        constexpr void Move(Vector2 offset) { position += offset; }
        constexpr void Rotate(float angleDegrees) { rotation += angleDegrees; }
        constexpr float GetRotationInRadians() const { return rotation * (PI / 180.0f); }
    public:
        Vector2 position;
        Vector2 scale;
        float rotation;
    };
}