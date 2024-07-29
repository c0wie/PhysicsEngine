#include "CollisionWorld.hpp"

namespace pe2d
{
    void CollisionWorld::AddObject(std::shared_ptr<CollisionObject> object)
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
        m_Objects.Insert(object, vertices);
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
        m_Objects.Resize(topLeftCorner, bottomRightCorner);
    }

    void CollisionWorld::UpdateQuadTreeContainer()
    {
        for(auto it = m_Objects.Begin(); it != m_Objects.End(); it++)
        {
            std::vector<Vector2> vertices;
            std::shared_ptr<pe2d::CircleCollider> circleCollider = std::static_pointer_cast<pe2d::CircleCollider>(it->item->GetCollider());
            if(circleCollider)
            {
                const float radius = circleCollider->GetRadius();
                vertices = algo::GetBoxVertices(pe2d::Vector2(radius, radius), it->item->GetTransform());
            }
            else
            {
                std::shared_ptr<pe2d::BoxCollider> boxCollider = std::static_pointer_cast<pe2d::BoxCollider>(it->item->GetCollider());
                vertices = algo::GetBoxVertices(boxCollider->GetSize(), it->item->GetTransform());
            }
            m_Objects.Relocate(it, vertices);
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