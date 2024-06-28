#include "../headers/PhysicsWorld.hpp"

namespace phy
{   
#pragma region COLLISION_WORLD 
    void CollisionWorld::SolveCollisions(std::vector<Collision> &collisions, float deltaTime)
    {
        for(int i = 0; i < m_Solvers.size(); i++)
        {
            m_Solvers[i]->Solve(collisions, deltaTime);
        }
    }

    void CollisionWorld::SendCollisionCallbacks(std::vector<Collision> &collisions, float deltaTime)
    {
    }

    void CollisionWorld::AddCollisionObject(std::shared_ptr<CollisionObject> obj)
    {
        m_Objects.push_back(obj);
    }

    void CollisionWorld::RemoveObject(std::shared_ptr<CollisionObject> object)
    {
        m_Objects.erase( std::remove(m_Objects.begin(), m_Objects.end(), object) );
    }

    void CollisionWorld::AddSolver(std::shared_ptr<Solver> &solver)
    {
        m_Solvers.push_back(solver);
    }
    
    void CollisionWorld::AddSolver(std::shared_ptr<PositionSolver> &solver)
    {
        m_Solvers.push_back(solver);
    }

    void CollisionWorld::AddSolver(std::shared_ptr<ImpulseSolver> &solver)
    {
        m_Solvers.push_back(solver);
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
                    (m_Objects[i]->GetTransform().get(), m_Objects[j]->GetCollider().get(), m_Objects[j]->GetTransform().get());

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
                    }
                }
            }
        }
        
        SolveCollisions(collisions, deltaTime);
        SendCollisionCallbacks(collisions, deltaTime);
        SendCollisionCallbacks(triggers, deltaTime);
    }

    void CollisionWorld::Draw(sf::RenderWindow &window)
    {
        for(auto obj : m_Objects)
        {
            obj->GetCollider()->Draw(window, obj->GetTransform().get());
        }
    }
#pragma endregion

#pragma region DYNAMICS_WORLD 

    void DynamicsWorld::AddRigidObject(std::shared_ptr<RigidObject> object)
    {
        m_Objects.push_back(object);
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
            //LogCall(object->GetForce().x, " ", object->GetForce().y, "\n");
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
            const Vector2 &vel = object->GetVelocity() + object->GetForce() * deltaTime;
            LogCall(vel.x, " ", vel.y, "\n");
            const Vector2 &pos = object->GetTransform()->position + object->GetVelocity() * deltaTime;
            //LogCall(pos.x, " ", pos.y, "\n");

            object->SetVelocity(vel);
            object->SetPosition(pos);
            object->SetForce(Vector2{});
        }
        LogCall("------------------\n");
    }

    void DynamicsWorld::Step(float deltaTime)
    {
        ApplyGravity();
        ResolveCollisions(deltaTime);
        MoveObjects(deltaTime);
    }
#pragma endregion
}