#include "PhysicsWorld.hpp"

namespace pe2d
{   
#pragma region COLLISION_WORLD 
    void CollisionWorld::AddCollisionObject(std::shared_ptr<CollisionObject> obj)
    {
        if(obj != nullptr)
        {
            m_Objects.push_back(obj);
        }
    }

    void CollisionWorld::RemoveObject(std::shared_ptr<CollisionObject> object)
    {
        m_Objects.erase( std::remove(m_Objects.begin(), m_Objects.end(), object) );
    }

    void CollisionWorld::AddSolver(std::shared_ptr<Solver> &solver)
    {
        if(solver != nullptr)
        {
            m_Solvers.push_back(solver);
        }
    }

    void CollisionWorld::RemoveSolver(std::shared_ptr<Solver> &solver)
    {
        m_Solvers.erase( std::remove(m_Solvers.begin(), m_Solvers.end(), solver) );
    }

    void CollisionWorld::ResolveCollisions(float deltaTime)
    {
        std::vector<Collision> collisions;
        std::vector<Collision> triggers;

        for(int i = 0; i < m_Objects.size(); i++)
        {
            for(int j = i + 1 ; j < m_Objects.size(); j++)
            {
                if(m_Objects[i] == m_Objects[j])
                {
                    break;
                }

                if(!m_Objects[i]->GetCollider() || !m_Objects[j]->GetCollider())
                {
                    continue;
                }
                
                FindCollisions(m_Objects[i], m_Objects[j], collisions, triggers);
            }
        }
        
        SolveCollisions(collisions, deltaTime);
        SendCollisionCallbacks(collisions, deltaTime);
        SendCollisionCallbacks(triggers, deltaTime);
    }
    
    void CollisionWorld::FindCollisions(std::shared_ptr<CollisionObject> objectA, std::shared_ptr<CollisionObject> objectB,
        std::vector<Collision> &collisions, std::vector<Collision> &triggers)
    {
        const CollisionPoints points = objectA->GetCollider()->TestCollision
            (objectA->GetTransform(), objectB->GetCollider().get(), objectB->GetTransform());
        Collision collision = Collision{objectA, objectB, points};

        if(points.HasCollision)
        {
            bool tigger = objectA->IsTrigger() || objectB -> IsTrigger();
            if(tigger)
            {
                triggers.push_back(collision);
            }
            else
            {
                collisions.push_back(collision);
            }
        }
    }

    void CollisionWorld::SolveCollisions(std::vector<Collision> &collisions, float deltaTime)
    {
        for(int i = 0; i < m_Solvers.size(); i++)
        {
            m_Solvers[i]->Solve(collisions, deltaTime);
        }
    }

    void CollisionWorld::SendCollisionCallbacks(std::vector<Collision> &collisions, float deltaTime)
    {
        for(int i = 0; i < collisions.size(); i++)
        {
            collisions[i].GetObjectA()->OnCollision(collisions[i], deltaTime);
            collisions[i].GetObjectB()->OnCollision(collisions[i], deltaTime);
        }
    }
#pragma endregion

#pragma region DYNAMICS_WORLD 
    void DynamicsWorld::AddRigidObject(std::shared_ptr<RigidObject> object)
    {
        m_Objects.push_back(object);
    }
    
    void DynamicsWorld::Step(float deltaTime)
    {
        ApplyGravity();
        MoveObjects(deltaTime);
        ResolveCollisions(deltaTime);
    }
    
    void DynamicsWorld::ApplyGravity()
    {   
        for(int i = 0; i < m_Objects.size(); i++)
        {
            std::shared_ptr<RigidObject> object;
            object = std::dynamic_pointer_cast<RigidObject>(m_Objects[i]);
            if(object == nullptr)
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
            std::shared_ptr<RigidObject> object;
            object = std::dynamic_pointer_cast<RigidObject>(m_Objects[i]);
            if(object == nullptr)
            {
                continue;
            }
            
            Vector2 vel = object->GetVelocity() + object->GetForce() * deltaTime;
            vel.x = vel.x < m_MAX_VELOCITY? vel.x : m_MAX_VELOCITY;
            vel.y = vel.y < m_MAX_VELOCITY? vel.y : m_MAX_VELOCITY;
            const Vector2 &pos = object->GetPosition() + object->GetVelocity() * deltaTime;

            object->SetVelocity(vel);
            object->SetPosition(pos);
            object->SetForce(Vector2{});
        }
    }
#pragma endregion
}