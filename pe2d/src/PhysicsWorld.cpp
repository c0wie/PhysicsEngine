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
            for(int j = 0; j < m_Objects.size(); j++)
            {
                if(m_Objects[i] == m_Objects[j])
                {
                    break;
                }

                if(m_Objects[i]->GetCollider() == nullptr || m_Objects[j]->GetCollider() == nullptr)
                {
                    continue;
                }


                CollisionPoints points = m_Objects[i]->GetCollider()->TestCollision
                    (m_Objects[i]->GetTransform(), m_Objects[j]->GetCollider().get(), m_Objects[j]->GetTransform());

                if(points.HasCollision)
                {
                    bool tigger = m_Objects[i]->IsTrigger() || m_Objects[j] -> IsTrigger();
                    if(tigger)
                    {
                        triggers.emplace_back(m_Objects[i], m_Objects[j], points);
                    }
                    else
                    {
                        collisions.emplace_back(m_Objects[i], m_Objects[j], points);
                        LogCall("Collision detected\n");
                    }
                }
            }
        }
        
        SolveCollisions(collisions, deltaTime);
        SendCollisionCallbacks(collisions, deltaTime);
        SendCollisionCallbacks(triggers, deltaTime);
    }
    
    void CollisionWorld::SolveCollisions(std::vector<Collision> &collisions, float deltaTime)
    {
        for(int i = 0; i < m_Solvers.size(); i++)
        {
            m_Solvers[i]->Solve(collisions, deltaTime);
        }
    }

    void CollisionWorld::SendCollisionCallbacks(const std::vector<Collision> &collisions, float deltaTime)
    {
        for(int i = 0; i < collisions.size(); i++)
        {
            auto callbackA = collisions[i].GetObjectA()->OnCollision();
            auto callbackB = collisions[i].GetObjectB()->OnCollision();
            if(callbackA)
            {
                callbackA( collisions[i], deltaTime );
            }
            if(callbackB)
            {
                callbackB( collisions[i], deltaTime );
            }
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