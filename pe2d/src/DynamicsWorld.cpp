#include "DynamicsWorld.hpp"

namespace pe2d 
{   
    void DynamicsWorld::Step(float deltaTime)
    {
        ApplyGravity();
        ResolveCollisions(deltaTime);
        MoveObjects(deltaTime);
    }
    
    void DynamicsWorld::AddRigidObject(std::shared_ptr<RigidObject> object)
    {
        if(!object)
        {
            ASSERT("Unvalid object");
        }
        const unsigned int ID = object->GetID();
        if(m_Objects.find(ID) != m_Objects.end())
        {
            ASSERT("Can't assign same ID to more than one object in CollisionWorld");
        }
        m_Objects[object->GetID()] = object;
    }
    
    void DynamicsWorld::ResolveCollisions(float deltaTime)
    {
        // could pass my own allocator with memory arena for example
        std::vector<Collision> collisions;
        collisions.reserve(m_Objects.size() * 0.8);
        
        std::vector<Collision> triggers;
        triggers.reserve(m_Objects.size() * 0.2);

        if(m_IsGridOn)
        {
            m_Grid.Update(m_Objects);
            auto pairs = m_Grid.GetCollisionPairs();
            for(auto it = pairs.begin(); it != pairs.end(); it++)
            {
                FindCollisions(m_Objects.at(it->first), m_Objects.at(it->second), collisions, triggers);
            }
        }
        else
        {
            for(auto itA = m_Objects.begin(); itA != m_Objects.end(); itA++)
            {
                for(auto itB = m_Objects.begin(); itB != m_Objects.end(); itB++)
                {
                    if(itA == itB)
                    {
                        break;
                    }
                    FindCollisions(itA->second, itB->second, collisions, triggers);
                }
            }
        }
        SolveCollisions(collisions, deltaTime);
        ApplyFriction(collisions, deltaTime);
        SendCollisionCallbacks(collisions, deltaTime);
        SendCollisionCallbacks(triggers, deltaTime);
    }   

    void DynamicsWorld::ApplyGravity()
    {   
        for(auto it = m_Objects.begin(); it != m_Objects.end(); it++)
        {
            if(!it->second->IsRigid())
            {
                continue;
            }
            std::shared_ptr<RigidObject> object = std::dynamic_pointer_cast<RigidObject>(it->second);
            // 2nd Newton's law F = ma
            object->AddForce( object->GetGravity() * object->GetMass() );
        }
    }

    void DynamicsWorld::ApplyFriction(std::vector<Collision> &collisions, float deltaTime)
    {
        bool wasCollision = false;
        for(auto &collision : collisions)
        {
            if(collision.GetObjectA()->GetID() == 24 && collision.GetObjectB()->GetID() == 420)
            {
                wasCollision = true;
            }
            const std::shared_ptr<CollisionObject> objectA = collision.GetObjectA();
            const std::shared_ptr<CollisionObject> objectB = collision.GetObjectB();
            const CollisionPoints &points = collision.GetPoints();
            if(!objectA->IsRigid() && !objectB->IsRigid())
            {
                continue;
            }

            std::shared_ptr<RigidObject> rigidBodyA, rigidBodyB;
            Vector2 velocityA, velocityB;
            float dynamicFrictionA, dynamicFrictionB;
            float inverseMassA, inverseMassB;

            if( objectA->IsRigid() )
            {
                rigidBodyA = std::static_pointer_cast<RigidObject>(objectA);
                velocityA = rigidBodyA->GetVelocity();
                dynamicFrictionA = rigidBodyA->GetDynamicFriction();
                inverseMassA = rigidBodyA->GetInvMass();
            }
            else
            {
                rigidBodyA = nullptr;
                velocityA = Vector2(0.0f, 0.0f);
                dynamicFrictionA = 0.0f;
                inverseMassA = 0.0f;
            }
            if( objectB->IsRigid() )
            {
                rigidBodyB = std::static_pointer_cast<RigidObject>(objectB);
                velocityB = rigidBodyB->GetVelocity();
                dynamicFrictionB = rigidBodyB->GetDynamicFriction();
                inverseMassB = rigidBodyB->GetInvMass();
            }
            else
            {
                rigidBodyB = nullptr;
                velocityB = Vector2(0.0f, 0.0f);
                dynamicFrictionB = 0.0f;
                inverseMassB = 0.0f;
            }

            const Vector2 relativeVelocity = velocityB - velocityA;
            const float coefficientOfDynamicFriction = (dynamicFrictionA + dynamicFrictionB) / 2.0f;
            const float velocityAlongNormal = relativeVelocity.dot(points.Normal);

            const float impulseScalar = velocityAlongNormal / (inverseMassA + inverseMassB);

            const Vector2 impulse = points.Normal * impulseScalar;
            const Vector2 tangent = (relativeVelocity - points.Normal * velocityAlongNormal).normalized();

            float frictionImpulseScalar = (relativeVelocity.dot(tangent) * -1.0f) / (inverseMassA + inverseMassB);

            const float maxFriction = coefficientOfDynamicFriction * impulseScalar;
            if(std::abs(frictionImpulseScalar) > maxFriction)
            {
                frictionImpulseScalar = maxFriction * (frictionImpulseScalar > 0.0f ? -1.0f : 1.0f);
            }

            const Vector2 frictionImpulse = tangent * frictionImpulseScalar;

            if(rigidBodyA)
            {
                const Vector2 force = velocityA - impulse * inverseMassA;
                rigidBodyA->AddForce(force - frictionImpulse * inverseMassA);
            }
            if(rigidBodyB)
            {
                const Vector2 force = velocityB - impulse * inverseMassB;
                rigidBodyB->AddForce(force - frictionImpulse * inverseMassB);
            }
        }
        isColliding = wasCollision;
    }

    void DynamicsWorld::MoveObjects(float deltaTime)
    {
        for(auto it = m_Objects.begin(); it != m_Objects.end(); it++)
        {
            if(!it->second->IsRigid())
            {
                continue;
            }
            std::shared_ptr<RigidObject> object = std::dynamic_pointer_cast<RigidObject>(it->second);
            const Vector2 acceleration = object->GetForce() / object->GetMass();
            Vector2 newVel = object->GetVelocity() + acceleration * deltaTime;
            object->Move(object->GetVelocity() * deltaTime + (acceleration * std::pow(deltaTime, 2) * 0.5));
            object->SetVelocity(newVel);
            object->SetForce(Vector2(0.0f, 0.0f));
        }
    }
}