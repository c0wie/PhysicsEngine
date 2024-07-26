#pragma once

#include <memory>
#include <functional>

#include "Collider.hpp"

namespace pe2d
{
    class Collision;
    // class that holds information about object without dynamics
    class CollisionObject
    {
    public:
        CollisionObject(unsigned int ID, std::shared_ptr<Collider> collider, Transform transform, bool isTrigger, bool isMovable,
            std::function<void(Collision, float)> collisionCallback);
        CollisionObject(const CollisionObject &other);
        CollisionObject(CollisionObject &&other);
        CollisionObject& operator=(const CollisionObject &other);
        CollisionObject& operator=(CollisionObject &&other);
        virtual ~CollisionObject() = default;
    public:
        constexpr unsigned int GetID() const { return m_ID; }
        std::shared_ptr<Collider> GetCollider() const { return m_Collider; }
        Vector2 GetBounadingBox() const;
        constexpr Vector2 GetPosition() const { return m_Transform.position; }
        constexpr Vector2 GetScale() const { return m_Transform.scale; }
        constexpr float GetRotation() const { return m_Transform.rotation; }
        constexpr Transform GetTransform() const { return m_Transform; }
        constexpr bool IsTrigger() const { return m_IsTrigger; }
        constexpr bool IsMovable() const { return m_IsMovable; }
        void OnCollision(Collision &collision, float deltaTime) const;
        
        constexpr void SetPosition(const Vector2 &pos) { m_Transform.position = pos; }
        constexpr void SetScale(const Vector2 &scale) 
        { 
            if(scale.x < 0.0f || scale.y < 0.0f)
            {
                ASSERT("HOW SCALE COULD BE NEGATIVE NUMBER");
            }
            m_Transform.scale = scale; 
        } 
        constexpr void SetRotation(float angle) { m_Transform.rotation = angle; }
        void SetCollider(const std::shared_ptr<BoxCollider> collider)
        {
            if(collider)
            {
                m_Collider = collider;
            }
        }
        void SetCollider(const std::shared_ptr<CircleCollider> collider)
        {
            if(collider)
            {
                m_Collider = collider;
            }
        }
        constexpr void SetTransform(Transform transform) { m_Transform = transform; }
        constexpr void SetTrigger(bool isTrigger) { m_IsTrigger = isTrigger; }
        constexpr void SetMovability (bool isMovable) { m_IsMovable = isMovable; }
        void SetCollisionCallback(std::function<void(Collision, float)> collisionCallback) { m_OnCollision = collisionCallback; }
        constexpr void Move(const Vector2 &offset) { m_Transform.Move(offset); }
        constexpr void Rotate(float angle) { m_Transform.Rotate(angle); }
        
    public:
    protected:
        unsigned int m_ID;
        std::shared_ptr<Collider> m_Collider;
        Transform m_Transform;
        bool m_IsTrigger;
        bool m_IsMovable;
        std::function<void(Collision, float)> m_OnCollision;
    };
}