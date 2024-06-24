#pragma once

#include "Vector2.hpp"

namespace phy
{
    struct Transform
    {
    private:
        Vector2 m_Position;
        Vector2 m_Scale;
        Vector2 m_Rotation;
    public:
        Transform();
        Transform(const Vector2 &position, const Vector2 &scale, const Vector2 &rotation);
        Transform(const Transform &other);
        Transform(Transform &&other) noexcept;
        Transform& operator=(const Transform &other);
        Transform& operator=(Transform &&other) noexcept;

        Vector2 GetPosition() const;
        Vector2 GetScale() const;
        Vector2 GetRotation() const;
        void SetPosition(const Vector2 &position);
        void SetScale(const Vector2 &scale);
        void SetRotation(const Vector2 &rotation);
    };
}