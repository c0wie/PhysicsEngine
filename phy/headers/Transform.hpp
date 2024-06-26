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
        constexpr Transform() :
            m_Position(Vector2{0.0f, 0.0f}), m_Scale(Vector2{0.0f, 0.0f}), m_Rotation(Vector2{0.0f, 0.0f}) {}
        constexpr Transform(const Vector2 &position, const Vector2 &scale, const Vector2 &rotation) :
            m_Position(position), m_Scale(scale), m_Rotation(rotation) {}
        constexpr Transform(const Transform &other) = default;
        constexpr Transform(Transform &&other) noexcept :
            m_Position(other.m_Position), m_Scale(other.m_Scale), m_Rotation(other.m_Rotation)
        {
            other.m_Position = Vector2{};
            other.m_Scale = Vector2{};
            other.m_Rotation = Vector2{};
        }
        constexpr Transform& operator=(const Transform &other) = default;
        constexpr Transform& operator=(Transform &&other) noexcept
        {
            if(this == &other)
            {
                return *this;
            }
            m_Position = other.m_Position;
            m_Scale = other.m_Scale;
            m_Rotation = other.m_Rotation;
            other.m_Position = Vector2{};
            other.m_Scale = Vector2{};
            other.m_Rotation = Vector2{};
            return *this;
        }

        Vector2 GetPosition() const;
        Vector2 GetScale() const;
        Vector2 GetRotation() const;
        void SetPosition(const Vector2 &position);
        void SetScale(const Vector2 &scale);
        void SetRotation(const Vector2 &rotation);

        void Move(const Vector2 &offset);
    };
}