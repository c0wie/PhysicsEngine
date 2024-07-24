#include "DynamicsWorld.hpp"

namespace pe2d 
{   
    void DynamicsWorld::AddRigidObject(std::shared_ptr<RigidObject> obj)
    {
        if(!obj)
        {
            ASSERT("Unvalid object beeing passed to CollisionWorld");
        }
        const unsigned int ID = obj->GetID();
        if(m_Objects.find(ID) != m_Objects.end())
        {
            ASSERT("This index is already taken");
        }
        m_Objects[ID] = obj;
    }
    
    void DynamicsWorld::Step(float deltaTime)
    {
        ApplyGravity();
        ResolveCollisions(deltaTime);
        MoveObjects(deltaTime);
    }
    
    void DynamicsWorld::ApplyGravity()
    {   
        for(auto [index, object] : m_Objects)
        {
            std::shared_ptr<RigidObject> obj = std::dynamic_pointer_cast<RigidObject>(object);
            if(!obj)
            {
                continue;
            }
            obj->SetForce( obj->GetGravity() * obj->GetMass() );
        }
    }

    void DynamicsWorld::MoveObjects(float deltaTime)
    {
        for(auto [index, object] : m_Objects)
        {
            std::shared_ptr<RigidObject> obj = std::dynamic_pointer_cast<RigidObject>(object);
            if(!obj)
            {
                continue;
            }
            
            Vector2 vel = obj->GetVelocity() + obj->GetForce() * deltaTime;
            vel.x = vel.x < m_MAX_VELOCITY? vel.x : m_MAX_VELOCITY;
            vel.y = vel.y < m_MAX_VELOCITY? vel.y : m_MAX_VELOCITY;
            const Vector2 &pos = obj->GetPosition() + obj->GetVelocity() * deltaTime;

            obj->SetVelocity(vel);
            obj->SetPosition(pos);
            obj->SetForce( Vector2(0.0f, 0.0f) );
        }
    }
}