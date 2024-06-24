#include <CollisionObject.hpp>

namespace phy
{
    struct RigidObject : public CollisionObject
    {
    private:
        float m_Mass;
        Vector2 m_Velocity;
        Vector2 m_Force;
        Vector2 m_Gravity;          // Gravitional acceleration
        bool m_TakesGravity;        // If the rigidobject will take gravity from the world
        float m_StaticFriction;     // Static friction coefficient
        float m_DynamicFriction;    // Dynamic friction coefficient
        float m_Restitution;        // Elasticy of collision
    public:
        RigidObject();
        RigidObject(std::shared_ptr<Collider> collider, std::shared_ptr<Transform> transform, float trigger, 
            float mass, const Vector2 &velocity, const Vector2 &force, const Vector2 &gravity,
            bool takesGravity, float staticFriction, float dynamicFriction, float restitution);
        RigidObject(const RigidObject &other);
        RigidObject(RigidObject &&other);
        RigidObject& operator=(const RigidObject &other);
        RigidObject& operator=(RigidObject &&other);

        float GetMass() const;
        Vector2 GetVelocity() const;
        Vector2 GetForce() const;
        Vector2 GetGravity() const;
        float GetStaticFriction() const;
        float GetDynamicFriction() const;
        float GetRestitution() const;
        void SetMass(float mass);
        void SetVelocity(const Vector2 &velocity);
        void SetForce(const Vector2 &force);
        void SetStaticFriction(float staticFriction);
        void SetDynamicFriction(float dynamicDriction);
        void SetRestitution(float restitution);

    };
}