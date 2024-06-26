#include "../headers/Transform.hpp"

namespace phy
{

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