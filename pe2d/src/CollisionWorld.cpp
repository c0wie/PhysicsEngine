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
            UpdateGrid();
            auto pairs = m_PartitioningSystem.GetCollisionPairs();
            for(auto &[a, b] : pairs)
            {
                FindCollisions(a, b, collisions, triggers);
            }
            m_PartitioningSystem.Clear();
        }
        else
        {
            for(auto [IDA, objectA] : m_Objects)
            {
                for(auto [IDB, objectB] : m_Objects)
                {
                    if(objectA == objectB)
                    {
                        break;
                    }
                    if(!objectA->GetCollider() || !objectB->GetCollider()) // both have colliders
                    {
                        continue;
                    }

                    FindCollisions(objectA, objectB, collisions, triggers);
                }
            }
        }

        SolveCollisions(collisions, deltaTime);
        SendCollisionCallbacks(collisions, deltaTime);
        SendCollisionCallbacks(triggers, deltaTime);
    }
    
    void CollisionWorld::SetPartitioningSystem(Vector2 topLeftCorner, Vector2 bottomRightCorner, unsigned int maxDepth)
    {
        m_PartitioningSystem = QuadTree<std::shared_ptr<CollisionObject>>(topLeftCorner, bottomRightCorner, maxDepth);
        m_IsPartitioningSystemOn = true;
    }

    void CollisionWorld::RemovePartitiongSystem()
    {  
        if(m_IsPartitioningSystemOn)
        {
            m_PartitioningSystem = QuadTree<std::shared_ptr<CollisionObject>>();
            m_IsPartitioningSystemOn = false;
        }
    }

    void CollisionWorld::UpdateGrid()
    {
        for(const auto [index, object] : m_Objects)
        {
            std::vector<Vector2> vertices;
            std::shared_ptr<pe2d::CircleCollider> circleCollider = std::static_pointer_cast<pe2d::CircleCollider>(object->GetCollider());
            if(circleCollider)
            {
                const float radius = circleCollider->GetRadius();
                vertices = algo::GetBoxVertices(pe2d::Vector2(radius, radius), object->GetTransform());
            }
            else
            {
                std::shared_ptr<pe2d::BoxCollider> boxCollider = std::static_pointer_cast<pe2d::BoxCollider>(object->GetCollider());
                vertices = algo::GetBoxVertices(boxCollider->GetSize(), object->GetTransform());
            }
            m_PartitioningSystem.Insert(object, vertices);
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