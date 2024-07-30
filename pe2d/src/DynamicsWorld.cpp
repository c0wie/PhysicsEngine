#include "DynamicsWorld.hpp"

namespace pe2d 
{   
    void DynamicsWorld::Step(float deltaTime)
    {
        ApplyGravity();
        ResolveCollisions(deltaTime);
        MoveObjects(deltaTime);
    }
    
    void DynamicsWorld::ApplyGravity()
    {   
        for(auto it = m_Objects.Begin(); it != m_Objects.End(); it++)
        {
            std::shared_ptr<RigidObject> object = std::dynamic_pointer_cast<RigidObject>(it->item);
            // checks if object is RigidBody
            if(!object)
            {
                continue;
            }
            object->SetForce( object->GetGravity() * object->GetMass() );
        }
    }

    void DynamicsWorld::MoveObjects(float deltaTime)
    {
        for(auto it = m_Objects.Begin(); it != m_Objects.End(); it++)
        {
            std::shared_ptr<RigidObject> object = std::dynamic_pointer_cast<RigidObject>(it->item);
            // checks if object is rigidObject
            if(!object)
            {
                continue;
            }
            Vector2 vel = object->GetVelocity() + object->GetForce() * deltaTime;
            vel.x = std::min(vel.x, m_MAX_VELOCITY);
            vel.y = std::min(vel.x, m_MAX_VELOCITY);
            object->SetVelocity(vel);
            object->Move(object->GetVelocity() * deltaTime);
            object->SetForce(Vector2(0.0f, 0.0f));

        }
    }
}