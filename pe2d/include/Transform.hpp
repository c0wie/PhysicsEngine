#pragma once

#include "Vector2.hpp"
#include "Math.hpp"

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
        constexpr Transform() = default;
        constexpr Transform(Vector2 position, Vector2 scale, float rotation) : 
            position(position), scale(scale), rotation(rotation) {}
    public:
        constexpr void Move(Vector2 offset) { position += offset; }
        constexpr void Rotate(float angleDegrees) { rotation += angleDegrees; }
        constexpr float GetRotationInRadians() const { return rotation * (pe2dMath::PI / 180.0f); }
    public:
        Vector2 position{0.0f, 0.0f};
        Vector2 scale{1.0f, 1.0f};
        float rotation{0.0f};
    };
}