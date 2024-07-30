#include "CollisionWorld.hpp"

namespace pe2d
{
    void CollisionWorld::AddObject(std::shared_ptr<CollisionObject> object)
    {
        if(!object)
        {
            ASSERT("Unvalid object");
        }
        m_Objects.Insert(object, object->GetBounadingBox());
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
        collisions.reserve(m_Objects.Size() * 0.8);
        
        std::vector<Collision> triggers;
        triggers.reserve(m_Objects.Size() * 0.2);


        UpdateQuadTreeContainer();
        auto pairs = m_Objects.GetCollisionPairs();
        for(auto &[a, b] : pairs)
        {
            FindCollisions(a->item, b->item, collisions, triggers);
        }
        m_Objects.Clear();

        SolveCollisions(collisions, deltaTime);
        SendCollisionCallbacks(collisions, deltaTime);
        SendCollisionCallbacks(triggers, deltaTime);
    }
    
    void CollisionWorld::Resize(Vector2 topLeftCorner, Vector2 bottomRightCorner)
    {
        std::list<std::shared_ptr<CollisionObject>> backup;
        for(auto it = m_Objects.Begin(); it != m_Objects.End(); it++)
        {
            backup.push_back(it->item);
        }
        m_Objects.Resize(topLeftCorner, bottomRightCorner);
        for(auto it = backup.begin(); it != backup.end(); it++)
        {
            auto object = *it;
            m_Objects.Insert(object, object->GetBounadingBox());
        }
    }

    void CollisionWorld::UpdateQuadTreeContainer()
    {
        for(auto it = m_Objects.Begin(); it != m_Objects.End(); it++)
        {
            m_Objects.Relocate(it, it->item->GetBounadingBox());
        }
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