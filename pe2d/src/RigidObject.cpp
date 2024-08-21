#include "RigidObject.hpp"
#include "Algo.hpp"

namespace pe2d
{
    RigidObject::RigidObject(size_t ID, std::shared_ptr<Collider> collider, Transform transform, float mass,
                            Vector2 velocity, Vector2 gravity, bool isStatic, 
                            float staticFriction, float dynamicFriction, float restitiution) :
        m_ID(ID),
        m_Collider(collider),
        m_Transform(transform),
        m_Velocity(velocity),
        m_Gravity(gravity),
        m_IsStatic(isStatic),
        m_Force(Vector2(0.0f, 0.0f))
    {  
        SetMass(mass);
        SetStaticFriction(staticFriction);
        SetDynamicFriction(dynamicFriction);
        SetRestitution(restitiution);
    }

    RigidObject::RigidObject(const RigidObject &other):
        m_ID(other.m_ID),
        m_Collider(other.m_Collider),
        m_Transform(other.m_Transform),
        m_Mass(other.m_Mass),
        m_Velocity(other.m_Velocity),
        m_Force(other.m_Force),
        m_Gravity(other.m_Gravity),
        m_RotationalInertia(other.m_RotationalInertia),
        m_IsStatic(other.m_IsStatic),
        m_StaticFriction(other.m_StaticFriction),
        m_DynamicFriction(other.m_DynamicFriction),
        m_Restitution(other.m_Restitution)
    {}

    RigidObject::RigidObject(RigidObject &&other) :
        m_ID(other.m_ID),
        m_Collider(other.m_Collider),
        m_Transform(other.m_Transform),
        m_Mass(other.m_Mass),
        m_Velocity(other.m_Velocity),
        m_Force(other.m_Force),
        m_Gravity(other.m_Gravity),
        m_RotationalInertia(other.m_RotationalInertia),
        m_StaticFriction(other.m_StaticFriction),
        m_DynamicFriction(other.m_DynamicFriction),
        m_Restitution(other.m_Restitution)
    {
        other.m_ID = 0;
        other.m_Collider = nullptr;
        other.m_Transform = Transform();
        other.m_Mass = 0.0f;
        other.m_Velocity = Vector2(0.0f, 0.0f);
        other.m_Force = Vector2(0.0f, 0.0f);
        other.m_Gravity = Vector2(0.0f, 0.0f);
        other.m_RotationalInertia = 0.0f;
        other.m_IsStatic = false;
        other.m_StaticFriction = 0.0f;
        other.m_DynamicFriction = 0.0f;
        other.m_Restitution = 0.0f;
    }

    RigidObject& RigidObject::operator=(const RigidObject &other) 
    {
        if(this == &other)
        {
            return *this;
        }
        m_ID = other.m_ID;
        m_Collider = other.m_Collider;
        m_Transform = other.m_Transform;
        m_Mass = other.m_Mass;
        m_Velocity = other.m_Velocity;
        m_Force = other.m_Force;
        m_Gravity = other.m_Gravity;
        m_RotationalInertia = other.m_RotationalInertia;
        m_IsStatic = other.m_IsStatic;
        m_StaticFriction = other.m_StaticFriction;
        m_DynamicFriction = other.m_DynamicFriction;
        m_Restitution = other.m_Restitution;
        return *this;
    }

    RigidObject& RigidObject::operator=(RigidObject &&other) 
    {
        if(this == &other)
        {
            return *this;
        }
        m_ID = other.m_ID;
        m_Collider = other.m_Collider;
        m_Transform = other.m_Transform;
        m_Mass = other.m_Mass;
        m_Velocity = other.m_Velocity;
        m_Force = other.m_Force;
        m_Gravity = other.m_Gravity;
        m_RotationalInertia = other.m_RotationalInertia;
        m_IsStatic = other.m_IsStatic;
        m_StaticFriction = other.m_StaticFriction;
        m_DynamicFriction = other.m_DynamicFriction;
        m_Restitution = other.m_Restitution;

        other.m_ID = 0;
        other.m_Collider = nullptr;
        other.m_Transform = Transform();
        other.m_Mass = 0.0f;
        other.m_Velocity = Vector2(0.0f, 0.0f);
        other.m_Force = Vector2(0.0f, 0.0f);
        other.m_Gravity = Vector2(0.0f, 0.0f);
        other.m_RotationalInertia = 0.0f;
        other.m_IsStatic = false;
        other.m_StaticFriction = 0.0f;
        other.m_DynamicFriction = 0.0f;
        other.m_Restitution = 0.0f;
        return *this;
    }

    //Returns collection of four vertices representing corners of a non rotated bounding box  
    std::array<Vector2, 4> RigidObject::GetAABB() const
    {
        std::shared_ptr<CircleCollider> circleCollider = std::dynamic_pointer_cast<CircleCollider>( m_Collider );

        if(circleCollider)
        {
            const float diameter = circleCollider->GetRadius() * 2;
            return algo::GetBoxVertices(Vector2(diameter, diameter), m_Transform);
        }
        
        std::shared_ptr<BoxCollider> boxCollider = std::dynamic_pointer_cast<BoxCollider>( m_Collider );
        const std::array<Vector2, 4> vertices = algo::GetBoxVertices(boxCollider->GetSize(), m_Transform);

        Vector2 topLeftCorner = Vector2(pe2dMath::INF, pe2dMath::INF);
        Vector2 botRightCorner = Vector2(pe2dMath::MIN, pe2dMath::MIN);
        for(const auto vertex : vertices)
        {
            topLeftCorner.x = std::min(topLeftCorner.x, vertex.x);
            topLeftCorner.y = std::min(topLeftCorner.y, vertex.y);
            botRightCorner.x = std::max(botRightCorner.x, vertex.x);    
            botRightCorner.y = std::max(botRightCorner.y, vertex.y);
        }
        const Vector2 topRightCorner = Vector2(botRightCorner.x, topLeftCorner.y);
        const Vector2 botLeftCorner = Vector2(topLeftCorner.x, botRightCorner.y);
        return std::array<Vector2, 4>{topLeftCorner, topRightCorner, botRightCorner, botLeftCorner};
    }
}