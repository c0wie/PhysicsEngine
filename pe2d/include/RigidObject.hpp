#pragma once

#include "Collider.hpp"
#include "Math.hpp"

namespace pe2d
{
    // class that holds information about object with dynamics
    class RigidObject
    {
    public:
        RigidObject(size_t ID, std::shared_ptr<Collider> collider, Transform transform, float mass,
                    Vector2 velocity, Vector2 gravity, bool isStatic, 
                    float staticFriction, float dynamicFriction, float restitiution);
        RigidObject(const RigidObject &other);
        RigidObject(RigidObject &&other);
        RigidObject& operator=(const RigidObject &other);
        RigidObject& operator=(RigidObject &&other);
        RigidObject() = default;
    public:
        constexpr unsigned int GetID() const { return m_ID; }
        std::shared_ptr<Collider> GetCollider() const { return m_Collider; }
        //Returns collection of four vertices representing corners of a non rotated bounding box.
        std::array<Vector2, 4> GetAABB() const;
        constexpr Vector2 GetPosition() const { return m_Transform.position; }
        constexpr Vector2 GetScale() const { return m_Transform.scale; }
        constexpr float GetRotation() const { return m_Transform.rotation; }
        constexpr float GetRotationInRadians() const { return m_Transform.GetRotationInRadians(); }
        constexpr Transform GetTransform() const { return m_Transform; }
        constexpr float GetMass() const { return m_Mass; }
        constexpr float GetInvMass() const 
        {
            if(m_IsStatic)
            {
                return 0.0f;
            }
            return (1.0f / m_Mass); 
        }
        constexpr Vector2 GetVelocity() const { return m_Velocity; }
        constexpr Vector2 GetForce() const { return m_Force; }
        constexpr Vector2 GetGravity() const { return m_Gravity; }
        constexpr float GetRotationalInertia() const { return m_RotationalInertia; }
        constexpr float GetInvRotationalInertia() const 
        {
            return 1.0f / m_RotationalInertia;            
        }
        constexpr bool IsStatic() const { return m_IsStatic; }
        constexpr float GetStaticFriction() const { return m_StaticFriction; }
        constexpr float GetDynamicFriction() const { return m_DynamicFriction; }
        constexpr float GetRestitution() const { return m_Restitution; }

        constexpr void SetPosition(Vector2 pos) { m_Transform.position = pos; }
        constexpr void Move(Vector2 offset) { m_Transform.Move(offset); }
        constexpr void SetScale(Vector2 scale) 
        { 
            if(scale.x < 0.0f || scale.y < 0.0f)
            {
                ASSERT("HOW SCALE COULD BE NEGATIVE NUMBER");
            }
            m_Transform.scale = scale; 
        } 
        constexpr void SetRotation(float angleDegrees) { m_Transform.rotation = angleDegrees; }
        constexpr void Rotate(float angleDegrees) { m_Transform.Rotate(angleDegrees); }
        void SetCollider(std::shared_ptr<Collider> collider)
        {
            if(!collider)
            {
                ASSERT("Unvalid collider");
            }
            m_Collider = collider;
        }
        constexpr void SetTransform(Transform transform) { m_Transform = transform; }
        constexpr void SetMass(float mass)
        {
            if(mass <= 0.0f)
            {
                ASSERT("Mass of object has to be greater than 0");
            }
            if(m_IsStatic)
            {
                m_Mass = pe2dMath::INF;
            }
            else
            {
                m_Mass = mass;
                m_RotationalInertia = CalculateRotationalInertia();
            }
        }
        constexpr void SetVelocity(Vector2 velocity)
        {
            m_Velocity = velocity;
        }
        constexpr void AddVelocity(Vector2 velocity)
        {
            m_Velocity += velocity;
        }
        constexpr void SetForce(Vector2 force)
        {
            m_Force = force;
        }
        constexpr void AddForce(Vector2 force)
        {
            m_Force += force;
        }
        constexpr void SetGravity(Vector2 gravity)
        {
            m_Gravity = gravity;
        }
        constexpr void SetStaticFriction(float staticFriction)
        {
            if(staticFriction < 0.0f || staticFriction > 1.0f)
            {
                ASSERT("Value of static friction have to be beetwen 0 and 1");
            }
            m_StaticFriction = staticFriction;
        }
        constexpr void SetDynamicFriction(float dynamicFriction)
        {  
            if(dynamicFriction < 0.0f || dynamicFriction > 1.0f)
            {
                ASSERT("Value of dynamic friction has to be beetwen 0 and 1");
            }
            m_DynamicFriction = dynamicFriction;
        }
        constexpr void SetRestitution(float restitution)
        {
            m_Restitution = restitution;
        }
    private:
        float CalculateRotationalInertia()
        {
            auto circleCollider = std::dynamic_pointer_cast<CircleCollider>(m_Collider);
            if(circleCollider)
            {
                const float radius = circleCollider->GetRadius();
                return 0.5f * m_Mass * radius * radius;
            }
            auto boxCollider = std::dynamic_pointer_cast<BoxCollider>(m_Collider);
            if(boxCollider)
            {
                const float width = boxCollider->GetSize().x;
                const float height = boxCollider->GetSize().y;
                return (1.0f / 12.0f) * m_Mass * (width * width + height * height);
            }
            else
            {
                ASSERT("Unvalid type of collider\n");
            }
        }
    private:
        size_t m_ID;
        std::shared_ptr<Collider> m_Collider;
        Transform m_Transform;
        float m_Mass;
        Vector2 m_Velocity;
        Vector2 m_Force;
        Vector2 m_Gravity;       
        float m_RotationalInertia;   
        bool m_IsStatic;
        float m_StaticFriction;     // Static friction coefficient [in range 0 - 1] 
        float m_DynamicFriction;    // Dynamic friction coefficient [in range 0 - 1]
        float m_Restitution;        // Elasticy of collision [in range 0 - 1] 
    };
}