#include "../headers/Transform.hpp"

namespace phy
{
    Transform::Transform() :
        m_Position(Vector2{0.0f, 0.0f}), m_Scale(Vector2{0.0f, 0.0f}), m_Rotation(Vector2{0.0f, 0.0f}) {};

    Transform::Transform(const Vector2 &position, const Vector2 &scale, const Vector2 &rotation) :
        m_Position(position), m_Scale(scale), m_Rotation(rotation) {};

    Transform::Transform(const Transform &other) :
        m_Position(other.m_Position), m_Scale(other.m_Scale), m_Rotation(other.m_Rotation) {};

    Transform::Transform(Transform &&other) noexcept :
        m_Position(other.m_Position), m_Scale(other.m_Scale), m_Rotation(other.m_Rotation)
    {
        other.m_Position = Vector2{};
        other.m_Scale = Vector2{};
        other.m_Rotation = Vector2{};
    };

    Transform& Transform::operator=(const Transform &other)
    {
        if(this == &other)
        {
            return *this;
        }
        m_Position = other.m_Position;
        m_Scale = other.m_Scale;
        m_Rotation = other.m_Rotation;
        return *this;
    }

    Transform& Transform::operator=(Transform &&other) noexcept
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

    Vector2 Transform::GetPosition() const
    {
        return m_Position;
    }

    Vector2 Transform::GetScale() const
    {
        return m_Scale;
    }

    Vector2 Transform::GetRotation() const
    {
        return m_Rotation;
    }

    void Transform::SetPosition(const Vector2 &m_position)
    {
        m_Position = m_position;
    }

    void Transform::SetScale(const Vector2 &m_scale)
    {
        m_Scale = m_scale;
    }

    void Transform::SetRotation(const Vector2 &m_rotation)
    {
        m_Rotation = m_rotation;
    }

    void Transform::Move(const Vector2 &offset)
    {
        m_Position = Vector2{m_Position.x + offset.x, m_Position.y + offset.y};
    }
}