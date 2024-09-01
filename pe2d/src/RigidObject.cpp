#include "RigidObject.hpp"
#include "Algo.hpp"

namespace pe2d
{
    RigidObject::RigidObject(size_t ID, std::shared_ptr<Collider> collider, Transform transform, float mass,
                            Vector2 linearVelocity, float angularVelocity, Vector2 gravity, bool isStatic, 
                            float staticFriction, float dynamicFriction, float restitution) :
        m_Transform(transform),
        m_LinearVelocity(linearVelocity),
        m_Force({0.0f, 0.0f}),
        m_Gravity(gravity),
        m_Collider(collider),
        m_ID(ID),
        m_AngularVelocity(angularVelocity),
        m_IsStatic(isStatic)
    {  
        SetMass(mass);
        SetStaticFriction(staticFriction);
        SetDynamicFriction(dynamicFriction);
        SetRestitution(restitution);
    }

    RigidObject::RigidObject(size_t ID, std::shared_ptr<Collider> collider, Transform transform, float mass,
                            Vector2 gravity, bool isStatic, float staticFriction, float dynamicFriction, float restitution):
        m_Transform(transform),
        m_LinearVelocity({0.0f, 0.0f}),
        m_Force({0.0f, 0.0f}),
        m_Gravity(gravity),
        m_Collider(collider),
        m_ID(ID), 
        m_AngularVelocity(0.0f),
        m_IsStatic(isStatic)
    {
        SetMass(mass);
        SetStaticFriction(staticFriction);
        SetDynamicFriction(dynamicFriction);
        SetRestitution(restitution);
    }

    std::array<Vector2, 4> RigidObject::GetAABB() const
    {
        std::shared_ptr<CircleCollider> circleCollider = std::dynamic_pointer_cast<CircleCollider>( m_Collider );

        if(circleCollider)
        {
            const float diameter = circleCollider->GetRadius() * 2;
            return algo::GetBoxVertices(Vector2(diameter, diameter), Transform{m_Transform.position, m_Transform.scale, 0.0f});
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
    
    float RigidObject::CalculateRotationalInertia()
    {
        if(m_IsStatic)
        {
            return pe2dMath::INF;
        }
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
}
