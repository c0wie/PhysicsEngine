#include "PhysicsWorld.hpp"

namespace pe2d
{
    void PhysicsWorld::Step(float deltaTime)
    {
        for(int i = 0; i < m_Substeps; i++)
        {
            const float subtime = deltaTime / (float)m_Substeps;
            thingsToDraw.clear();
            ApplyGravity();
            ResolveCollisions(subtime);
            MoveObjects(subtime);
        }
    }

    void PhysicsWorld::AddObject(const RigidObject &object)
    {
        const unsigned int ID = object.GetID();
        if(m_Objects.find(ID) != m_Objects.end())
        {
            ASSERT("Can't assign same ID to more than one object in CollisionWorld");
        }
        m_Objects[ID] = object;
    }

    std::unordered_map<size_t, RigidObject>::iterator PhysicsWorld::RemoveObject(size_t ID)
    {
        auto it = m_Objects.find(ID);
        if (it != m_Objects.end()) 
        {
            return m_Objects.erase(it); 
        }
        return m_Objects.end(); 
    }

    void PhysicsWorld::AddSolver(std::shared_ptr<Solver> &solver)
    {
        if(!solver )
        {
            ASSERT("Unvalid solver");
        }
        m_Solvers.push_back(solver);
    }

    void PhysicsWorld::RemoveSolver(std::shared_ptr<Solver> &solver)
    {
        m_Solvers.erase( std::remove(m_Solvers.begin(), m_Solvers.end(), solver) );
    }

    void PhysicsWorld::ResolveCollisions(float deltaTime)
    {
        // could pass my own allocator with memory arena for example
        std::vector<Collision> collisions;
        collisions.reserve(m_Objects.size());
        
        if(m_IsGridOn)
        {
            m_Grid.Update(m_Objects);
            std::list<std::pair<size_t, size_t>> pairs = m_Grid.GetCollisionPairs();
            for(auto it = pairs.begin(); it != pairs.end(); it++)
            {
                FindCollisions(it->first, it->second, collisions);
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
                    FindCollisions(itA->first, itB->first, collisions);
                }
            }
        }
        //ApplyFriction(collisions);
        SolveCollisions(collisions, deltaTime);
    }
    
    void PhysicsWorld::AddGrid(Vector2 topLeftCorner, Vector2 bottomRightCorner, float cellSize)
    {
        m_Grid = Grid(topLeftCorner, bottomRightCorner, cellSize);
        m_IsGridOn = true;
    }

    void PhysicsWorld::RemoveGrid()
    {
        m_Grid = Grid();
        m_IsGridOn = false;
    }

    void PhysicsWorld::ResizeGrid(Vector2 topLeftCorner, Vector2 bottomRightCorner, float cellSize)
    {
        m_Grid = Grid(topLeftCorner, bottomRightCorner, cellSize);
    }

    void PhysicsWorld::FindCollisions(size_t IDA, size_t IDB, std::vector<Collision> &collisions)
    {
        RigidObject &A = m_Objects.at(IDA);
        RigidObject &B = m_Objects.at(IDB);

        if(A.IsStatic() && B.IsStatic())
        {
            return;
        }

        // check if objects are penetrating if so pull them apart
        CollisionPoints points = A.GetCollider()->TestCollision(A.GetTransform(), B.GetCollider(), B.GetTransform());
        if(points.HasCollision)
        {
            if(points.ContactCount == 1)
            {
                thingsToDraw.push_back(points.ContactPoint1);
            }
            else
            {
                thingsToDraw.push_back(points.ContactPoint1);
                thingsToDraw.push_back(points.ContactPoint2);
            }

            collisions.emplace_back(A, B, points);
        }
    }

    void PhysicsWorld::SolveCollisions(std::vector<Collision> &collisions, float deltaTime)
    {
        for(int i = 0; i < m_Solvers.size(); i++)
        {
            m_Solvers[i]->Solve(collisions, deltaTime);
        }
    }

    void PhysicsWorld::ApplyGravity()
    {
        for(auto it = m_Objects.begin(); it != m_Objects.end(); it++)
        {
            RigidObject &object = it->second;
            if(!object.IsStatic())
            {
                object.AddForce( object.GetGravity() * object.GetMass() );
            }
        }
    }

    void PhysicsWorld::ApplyFriction(std::vector<Collision> &collisions)
    {
        constexpr float FRICTION_SCALING_FACTOR = 30.0f;
        for(size_t i = 0; i < collisions.size(); i++)
        {
            Collision &collision = collisions[i];
            RigidObject &rigidObjectA = collision.GetObjectA();
            RigidObject &rigidObjectB = collision.GetObjectB();
            const CollisionPoints &points = collision.GetCollisionPoints();

            const Vector2 relativeVelocity = rigidObjectB.GetLinearVelocity() - rigidObjectA.GetLinearVelocity();
            const float coefficientOfStaticFriction = FRICTION_SCALING_FACTOR * 
                        (rigidObjectA.GetStaticFriction() + rigidObjectB.GetStaticFriction()) / 2.0f;

            const float coefficientOfDynamicFriction = FRICTION_SCALING_FACTOR * 
                        (rigidObjectA.GetDynamicFriction() + rigidObjectB.GetDynamicFriction()) / 2.0f;

            // perpendicular to normal, represent direciton in which friction will act
            Vector2 tangent = relativeVelocity - pe2dMath::Dot(relativeVelocity, points.Normal) * points.Normal;
            if(tangent == pe2d::Vector2(0.0f, 0.0f))
            {
                continue;
            }
            else
            {
                tangent = pe2dMath::Normalize(tangent);
            }
            float jt = pe2dMath::Dot(relativeVelocity, tangent);
            jt /= (rigidObjectA.GetInvMass() + rigidObjectB.GetInvMass());

            Vector2 frictionImpulse;
            const float j = pe2dMath::Dot(relativeVelocity, points.Normal);
            if(abs(jt) <= j * coefficientOfStaticFriction)
            {
                frictionImpulse = jt * tangent;
            }
            else
            {
                frictionImpulse = -j * tangent * coefficientOfDynamicFriction;
            }
            
            rigidObjectA.AddForce(frictionImpulse);
            rigidObjectB.AddForce(frictionImpulse);
        }
    }

    void PhysicsWorld::MoveObjects(float deltaTime)
    {
        for(auto it = m_Objects.begin(); it != m_Objects.end(); it++)
        {
            RigidObject &object = it->second;
            const Vector2 acceleration = object.GetForce() * object.GetInvMass();
            Vector2 newVel = object.GetLinearVelocity() + acceleration * deltaTime;
            object.Move(object.GetLinearVelocity() * deltaTime + (acceleration * std::pow(deltaTime, 2) * 0.5));
            if(std::dynamic_pointer_cast<BoxCollider>(it->second.GetCollider()))
            {
                object.Rotate(object.GetAngularVelocity() * deltaTime * 50.0f);
            }
            else 
            {
                object.Rotate(object.GetAngularVelocity() * deltaTime);
            }
            object.SetLinearVelocity(newVel);
            object.SetForce(Vector2(0.0f, 0.0f));
        }
    }
}