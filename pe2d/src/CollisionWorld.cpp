#include "CollisionWorld.hpp"

namespace pe2d
{

    void CollisionWorld::AddCollisionObject(std::shared_ptr<CollisionObject> object)
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

    std::unordered_map<size_t, std::shared_ptr<CollisionObject>>::iterator CollisionWorld::RemoveObject(size_t ID)
    {
        auto it = m_Objects.find(ID);
        if (it != m_Objects.end()) 
        {
            return m_Objects.erase(it); 
        }
        return m_Objects.end(); 
    }

    void CollisionWorld::AddSolver(std::shared_ptr<Solver> &solver)
    {
        if(!solver )
        {
            ASSERT("Unvalid solver");
        }
        m_Solvers.push_back(solver);
    }

    void CollisionWorld::RemoveSolver(std::shared_ptr<Solver> &solver)
    {
        m_Solvers.erase( std::remove(m_Solvers.begin(), m_Solvers.end(), solver) );
    }

    void CollisionWorld::ResolveCollisions(float deltaTime)
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
        SendCollisionCallbacks(collisions, deltaTime);
        SendCollisionCallbacks(triggers, deltaTime);
    }
    
    void CollisionWorld::AddGrid(Vector2 topLeftCorner, Vector2 bottomRightCorner, float cellSize)
    {
        m_Grid = Grid(topLeftCorner, bottomRightCorner, cellSize);
        m_IsGridOn = true;
    }

    void CollisionWorld::RemoveGrid()
    {
        m_Grid = Grid();
        m_IsGridOn = false;
    }

    void CollisionWorld::ResizeGrid(Vector2 topLeftCorner, Vector2 bottomRightCorner, float cellSize)
    {
        m_Grid = Grid(topLeftCorner, bottomRightCorner, cellSize);
    }

    void CollisionWorld::FindCollisions(std::shared_ptr<CollisionObject> objectA, std::shared_ptr<CollisionObject> objectB,
        std::vector<Collision> &collisions, std::vector<Collision> &triggers)
    {
        const CollisionPoints points = objectA->GetCollider()->TestCollision
            (objectA->GetTransform(), objectB->GetCollider().get(), objectB->GetTransform());

        if(points.HasCollision)
        {
            const Collision collision = Collision(objectA, objectB, points);
            const bool tigger = objectA->IsTrigger() || objectB -> IsTrigger();
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
}