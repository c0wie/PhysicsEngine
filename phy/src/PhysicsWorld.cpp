#include "../headers/PhysicsWorld.hpp"

namespace phy
{   
#pragma region COLLISION_WORLD 
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


    void CollisionWorld::ResolveCollision(float delatTime)
    {
        std::vector<Collision> collisions;
        for(int i = 0; i < m_Objects.size(); i++)
        {
            for(int j = 0; j < m_Objects.size(); j++)
            {
                if(m_Objects[i]->GetCollider() == nullptr || m_Objects[j]->GetCollider() == nullptr)
                {
                    continue;
                }

                if(m_Objects[i] == m_Objects[j])
                {
                    continue;
                }

                CollisionPoints points = m_Objects[i]->GetCollider()->TestCollision
                    (m_Objects[i]->GetTransform().get(), m_Objects[j]->GetCollider().get(), m_Objects[j]->GetTransform().get());

                if(points.HasCollision)
                {
                    collisions.emplace_back(m_Objects[i], m_Objects[j], points);
                }
            }
        }
        
        for(int i = 0; i < m_Solvers.size(); i++)
        {
            m_Solvers[i]->Solve(collisions, delatTime);
        }
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
    
    void DynamicsWorld::Step(float deltaTime)
    {
        ResolveCollision(deltaTime);
        for(int i = 0; i < m_Objects.size(); i++)
        {
            // example of force
            std::shared_ptr<RigidObject> object;
            object = std::dynamic_pointer_cast<RigidObject>(m_Objects[i]);
            if(object == nullptr)
            {
                continue;
            }

            object->SetForce(Vector2{object->GetForce().x, object->GetMass() * m_Gravity});
            LogCall(object->GetForce().x, " ", object->GetForce().y, "\n");
            object->SetVelocity(object->GetForce() / (object->GetMass() * deltaTime) );
            LogCall(object->GetVelocity().x, " ", object->GetVelocity().y, "\n");
            object->GetTransform()->SetPosition(object->GetVelocity() * deltaTime);
            LogCall(object->GetTransform()->GetPosition().x, " ", object->GetTransform()->GetPosition().x, "\n");
            object->SetForce(Vector2{});
            
        }
    }
#pragma endregion
}