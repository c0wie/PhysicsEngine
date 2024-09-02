#pragma once

#include "Collider.hpp"
#include "Math.hpp"

namespace pe2d
{
    // class that holds information about object with dynamics
    class RigidObject
    {
    public:
        RigidObject() = default;
        RigidObject(size_t ID, std::shared_ptr<Collider> collider, Transform transform, float mass,
                    Vector2 linearVelocity,float angularVelocity, Vector2 gravity, bool isStatic, 
                    float staticFriction, float dynamicFriction, float restitution);
        RigidObject(size_t ID, std::shared_ptr<Collider> collider, Transform transform, float mass,
                    Vector2 gravity, bool isStatic, float staticFriction, float dynamicFriction, float restitution);
    public:
        constexpr unsigned int GetID() const { return m_ID; }
        std::shared_ptr<Collider> GetCollider() const { return m_Collider; }
        //Returns collection of four vertices representing corners of a non rotated bounding box.
        std::array<Vector2, 4> GetAABB() const;
        constexpr Vector2 GetPosition() const { return m_Transform.position; }
        constexpr Vector2 GetScale() const { return m_Transform.scale; }
        constexpr float GetRotation() const { return m_Transform.rotation; }
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
        constexpr Vector2 GetLinearVelocity() const { return m_LinearVelocity; }
        constexpr float GetAngularVelocity() const { return m_AngularVelocity; }
        constexpr Vector2 GetForce() const { return m_Force; }
        constexpr Vector2 GetGravity() const { return m_Gravity; }
        constexpr float GetRotationalInertia() const { return m_RotationalInertia; }
        constexpr float GetInvRotationalInertia() const 
        {
            if(m_IsStatic)
            {
                return 0.0f;
            }
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
        constexpr void SetRotation(float angleRadians) { m_Transform.rotation = angleRadians; }
        constexpr void Rotate(float angleRadians) { m_Transform.Rotate(angleRadians); }
        void SetCollider(std::shared_ptr<Collider> collider)
        {
            if(!collider)
            {
                ASSERT("Unvalid collider");
            }
            m_Collider = collider;
        }
        constexpr void SetTransform(Transform transform) { m_Transform = transform; }
        void SetMass(float mass)
        {
            if(mass <= 0.0f)
            {
                ASSERT("Mass of object has to be greater than 0");
            }
            m_Mass = m_IsStatic? pe2dMath::INF : mass;
            m_RotationalInertia = CalculateRotationalInertia();
        }
        constexpr void SetLinearVelocity(Vector2 linearVelocity) { m_LinearVelocity = linearVelocity; }
        constexpr void AddLinearVelocity(Vector2 linearVelocity) { m_LinearVelocity += linearVelocity; }
        constexpr void SetAngularVelocity(float angluarVelocity) { m_AngularVelocity = angluarVelocity; }
        constexpr void AddAngularVelocity(float angularVelocity) { m_AngularVelocity += angularVelocity; }
        constexpr void SetForce(Vector2 force) { m_Force = force; }
        constexpr void AddForce(Vector2 force) { m_Force += force; }
        constexpr void SetGravity(Vector2 gravity) { m_Gravity = gravity; }
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
            if(restitution < 0.0f || restitution > 1.0f)
            {
                ASSERT("Value of restitution has to be beetwen 0 and 1");
            }
            m_Restitution = restitution;
        }
    protected:
        float CalculateRotationalInertia();
    private:
        Transform m_Transform;
        Vector2 m_LinearVelocity{0.0f, 0.0f};
        Vector2 m_Force{0.0f, 0.0f};
        Vector2 m_Gravity{0.0f, 0.0f};
        std::shared_ptr<Collider> m_Collider;
        size_t m_ID {0U};
        float m_Mass{10.0f};
        float m_AngularVelocity{0.0f};
        float m_RotationalInertia{0.0f};
        float m_StaticFriction{0.0f};     // Static friction coefficient [in range 0 - 1] 
        float m_DynamicFriction{0.0f};    // Dynamic friction coefficient [in range 0 - 1]
        float m_Restitution{0.0f};        // Elasticy of collision [in range 0 - 1] 
        bool m_IsStatic{false};
    };
}