#include "../headers/PhysicsWorld.hpp"

namespace phy
{   
#pragma region COLLISION_WORLD 
    void CollisionWorld::AddObject(std::shared_ptr<CollisionObject> obj)
    {
        m_Objects.push_back(obj);
        LogCall("Object has been added");
    }

    void CollisionWorld::RemoveObject(std::shared_ptr<CollisionObject> object)
    {
        m_Objects.erase( std::remove(m_Objects.begin(), m_Objects.end(), object) );
        LogCall("Object has been romoved");
    }

    void CollisionWorld::AddSolver(std::shared_ptr<Solver> &solver)
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
           /* m_Objects[i]->Force.y += m_Objects[i]->Mass * m_Gravity;

            m_Objects[i]->Velocity += m_Objects[i]->Force / m_Objects[i]->Mass * deltaTime;
            m_Objects[i]->Transform->Position += m_Objects[i]->Velocity * deltaTime;

            m_Objects[i]->Force = Vector2(0.0f, 0.0f); */
        }
    }
#pragma endregion
}