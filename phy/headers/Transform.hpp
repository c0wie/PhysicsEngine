#pragma once

#include "Vector2.hpp"

namespace phy
{
    class Transform
    {
    public:
        Vector2 Position;
        Vector2 Scale;
    public:
        Transform();
        Transform(const Vector2 &position, const Vector2 &scale);
        Transform(const Transform &other);
        Transform(Transform &&other) noexcept;
        Transform& operator=(const Transform &other);
        Transform& operator=(Transform &&other) noexcept;
    };
}