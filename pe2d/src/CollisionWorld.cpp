#include "CollisionWorld.hpp"

namespace pe2d
{
    void CollisionWorld::AddCollisionObject(std::shared_ptr<CollisionObject> obj)
    {
        if(!obj)
        {
            ASSERT("Unvalid object beeing passed to CollisionWorld");
        }
        const unsigned int ID = obj->GetID();
        if(m_Objects.find(ID) != m_Objects.end())
        {
            ASSERT("This index is already taken");
        }
        m_Objects[ID] = obj;
    }

    void CollisionWorld::RemoveObject(unsigned int ID)
    {
        m_Objects.erase(ID);
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


        if(m_IsPartitioningSystemOn)
        {
            if(!m_PartitioningSystem.IsValid())
            {
                ASSERT("UNTIL YOU SETUP PARTITIONING SYSTEM YOU CAN'T USE IT");
            }
            UpdateGrid();
            std::vector<std::pair<unsigned int, unsigned int>> pairs = m_PartitioningSystem.GetCollisionPairs();
            std::cout << pairs.size() << '\n';
            for(auto &[a, b] : pairs)
            {
                FindCollisions(m_Objects[a], m_Objects[b], collisions, triggers);
            }
            m_PartitioningSystem.Clear();
        }
        else
        {
            for(auto itA = m_Objects.begin(); itA != m_Objects.end(); itA++)
            {
                for(auto itB = m_Objects.begin(); itB != m_Objects.end(); itB++)
                {
                    if(itA == itB) // unique pars
                    {
                        break;
                    }

                    if(!itA->second->GetCollider() || !itB->second->GetCollider()) // both have colliders
                    {
                        continue;
                    }

                    FindCollisions(itA->second, itB->second, collisions, triggers);
                }
            }
        }

        SolveCollisions(collisions, deltaTime);
        SendCollisionCallbacks(collisions, deltaTime);
        SendCollisionCallbacks(triggers, deltaTime);
    }
    
    void CollisionWorld::SetPartitioningSystem(Vector2 topLeftCorner, Vector2 bottomRightCorner, unsigned int maxDepth)
    {
        if(!m_IsPartitioningSystemOn)
        {
            ASSERT("Can't setup partitioninf system when it is disabled");
        }
        m_PartitioningSystem = QuadTree(topLeftCorner, bottomRightCorner, maxDepth);
    }

    void CollisionWorld::UpdateGrid()
    {
        for(int i = 0; i < m_Objects.size(); i++)
        {
            m_PartitioningSystem.Insert(m_Objects[i]);
        }
    }

    void CollisionWorld::FindCollisions(std::shared_ptr<CollisionObject> objectA, std::shared_ptr<CollisionObject> objectB,
        std::vector<Collision> &collisions, std::vector<Collision> &triggers)
    {
        const CollisionPoints points = objectA->GetCollider()->TestCollision
            (objectA->GetTransform(), objectB->GetCollider().get(), objectB->GetTransform());

        if(points.HasCollision)
        {
            Collision collision = Collision{objectA, objectB, points};
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
}