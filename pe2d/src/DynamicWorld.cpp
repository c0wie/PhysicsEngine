#include "DynamicsWorld.hpp"

namespace pe2d 
{   
    void DynamicsWorld::AddRigidObject(std::shared_ptr<RigidObject> object)
    {
        if(object != nullptr)
        {
            const unsigned int ID = object->GetID();
            if(m_Objects.at(ID))
            {
                ASSERT("OBJECT WITH THIS INDEX ALREADY EXIST");
            }
            m_Objects[ID] = object;
        }
    }
    
    void DynamicsWorld::Step(float deltaTime)
    {
        ApplyGravity();
        ResolveCollisions(deltaTime);
        MoveObjects(deltaTime);
    }
    
    void DynamicsWorld::ApplyGravity()
    {   
        for(int i = 0; i < m_Objects.size(); i++)
        {
            std::shared_ptr<RigidObject> object = std::dynamic_pointer_cast<RigidObject>(m_Objects[i]);;
            if(!object)
            {
                continue;
            }
            object->SetForce( object->GetGravity() * object->GetMass() );
        }
    }

    void DynamicsWorld::MoveObjects(float deltaTime)
    {
        for(int i = 0; i < m_Objects.size(); i++)
        {
            std::shared_ptr<RigidObject> object = std::dynamic_pointer_cast<RigidObject>(m_Objects[i]);
            if(!object)
            {
                continue;
            }
            
            Vector2 vel = object->GetVelocity() + object->GetForce() * deltaTime;
            vel.x = vel.x < m_MAX_VELOCITY? vel.x : m_MAX_VELOCITY;
            vel.y = vel.y < m_MAX_VELOCITY? vel.y : m_MAX_VELOCITY;
            const Vector2 &pos = object->GetPosition() + object->GetVelocity() * deltaTime;

            object->SetVelocity(vel);
            object->SetPosition(pos);
            object->SetForce(Vector2{0.0f, 0.0f});
        }
    }
}