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
            ASSERT("Can't assign same ID to more than one object in DynamicsWorld");
        }
        m_Objects[object->GetID()] = object;
        if(object->TakesGravity())
        {
            object->SetGravity(m_GRAVITY);
        }
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
        ApplyFriction(collisions);
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

    void DynamicsWorld::ApplyFriction(std::vector<Collision> &collisions)
    {
        constexpr float FRICTION_SCALING_FACTOR = 30.0f;
        isColliding = false;
        for(auto &collision : collisions)
        {
            const std::shared_ptr<CollisionObject> objectA = collision.GetObjectA();
            const std::shared_ptr<CollisionObject> objectB = collision.GetObjectB();
            const CollisionPoints &points = collision.GetPoints();
            if(!objectA->IsRigid() && !objectB->IsRigid())
            {
                continue;
            }

            if(objectA->GetID() == 24 && objectB->GetID() == 420)
            {
                isColliding = true;
            }
            std::shared_ptr<RigidObject> rigidBodyA, rigidBodyB;
            Vector2 velocityA, velocityB;
            float staticFrictionA, staticFrictionB;
            float dynamicFrictionA, dynamicFrictionB;
            float inverseMassA, inverseMassB;

            if( objectA->IsRigid() )
            {
                rigidBodyA = std::static_pointer_cast<RigidObject>(objectA);
                velocityA = rigidBodyA->GetVelocity();
                staticFrictionA = rigidBodyA->GetStaticFriction();
                dynamicFrictionA = rigidBodyA->GetDynamicFriction();
                inverseMassA = rigidBodyA->GetInvMass();
            }
            else
            {
                rigidBodyA = nullptr;
                velocityA = Vector2(0.0f, 0.0f);
                staticFrictionA = 0.0f;
                dynamicFrictionA = 0.0f;
                inverseMassA = 0.0f;
            }
            if( objectB->IsRigid() )
            {
                rigidBodyB = std::static_pointer_cast<RigidObject>(objectB);
                velocityB = rigidBodyB->GetVelocity();
                staticFrictionB = rigidBodyB->GetStaticFriction();
                dynamicFrictionB = rigidBodyB->GetDynamicFriction();
                inverseMassB = rigidBodyB->GetInvMass();
            }
            else
            {
                rigidBodyB = nullptr;
                velocityB = Vector2(0.0f, 0.0f);
                staticFrictionB = 0.0f;
                dynamicFrictionB = 0.0f;
                inverseMassB = 0.0f;
            }

            const Vector2 relativeVelocity = velocityB - velocityA;
            const float coefficientOfStaticFriction = FRICTION_SCALING_FACTOR * (staticFrictionA + staticFrictionB) / 2.0f;
            const float coefficientOfDynamicFriction = FRICTION_SCALING_FACTOR * (dynamicFrictionA + dynamicFrictionB) / 2.0f;

            // perpendicular to normal, represent direciton in which friction will act
            Vector2 tangent = relativeVelocity - relativeVelocity.dot(points.Normal) * points.Normal;
            if(tangent == pe2d::Vector2(0.0f, 0.0f))
            {
                continue;
            }
            else
            {
                tangent = tangent.normalized();
            }
            float jt = relativeVelocity.dot(tangent);
            jt /= (rigidBodyA->GetInvMass() + rigidBodyB->GetInvMass());

            Vector2 frictionImpulse;
            const float j = relativeVelocity.dot(points.Normal);
            if(abs(jt) <= j * coefficientOfStaticFriction)
            {
                frictionImpulse = jt * tangent;
            }
            else
            {
                frictionImpulse = -j * tangent * coefficientOfDynamicFriction;
            }
            
            rigidBodyA->AddForce(frictionImpulse);
            rigidBodyB->AddForce(frictionImpulse);
        }
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