#include "PhysicsWorld.hpp"

namespace pe2d
{
    void PhysicsWorld::Step(float deltaTime)
    {
        for(int i = 0; i < m_Substeps; i++)
        {
            const float subtime = deltaTime / (float)m_Substeps;
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
            const Vector2 normal = points.Normal;
            const float invMassA = rigidObjectA.GetInvMass();
            const float invMassB = rigidObjectB.GetInvMass();

            const int contactCount = points.ContactCount;
            const float coefficientOfStaticFriction = (rigidObjectA.GetStaticFriction() + rigidObjectB.GetStaticFriction()) * 0.5f;
            const float coefficientOfDynamicFriction = (rigidObjectA.GetDynamicFriction() + rigidObjectB.GetDynamicFriction()) * 0.5f;
            const Vector2 contactList[2] = { points.ContactPoint1, points.ContactPoint2 };

            Vector2 deltaLinearVelocityA = Vector2(0.0f, 0.0f);
            float deltaAngularVelocityA = 0.0f;
            Vector2 deltaLinearVelocityB = Vector2(0.0f, 0.0f);
            float deltaAngularVelocityB = 0.0f;

            for(int k = 0; k < contactCount; k++)
            {
                // vector pointing from center of mass of the objects to the contact points
                const Vector2 rA = contactList[k] - rigidObjectA.GetPosition();
                const Vector2 rB = contactList[k] - rigidObjectB.GetPosition();
                
                const Vector2 rAperp = pe2dMath::Perp(rA);
                const Vector2 rBperp = pe2dMath::Perp(rB);

                const Vector2 angularVelocityA = rAperp  * rigidObjectA.GetAngularVelocity();
                const Vector2 angularVelocityB = rBperp  * rigidObjectB.GetAngularVelocity();

                const Vector2 relativeVelocity = (rigidObjectA.GetLinearVelocity() + angularVelocityA) 
                                            - (rigidObjectB.GetLinearVelocity() + angularVelocityB); 

                Vector2 tangent = relativeVelocity - pe2dMath::Dot(relativeVelocity, normal) * normal;

                if(pe2dMath::NearlyEquel(tangent, {0.0f, 0.0f}, 0.05f))
                {
                    continue;
                }
                else
                {
                    tangent = pe2dMath::Normalize(tangent);
                }


                const float relativeVelocityAlongTang = pe2dMath::Dot(relativeVelocity, tangent);

                const float rAperpTang = pe2dMath::Dot(rAperp, tangent);
                const float rBperpTang = pe2dMath::Dot(rBperp, tangent);

                const float denominator = invMassA + invMassB 
                    + (rAperpTang * rAperpTang) * rigidObjectA.GetInvRotationalInertia()
                    + (rBperpTang * rBperpTang) * rigidObjectB.GetInvRotationalInertia();

                float jt = -relativeVelocityAlongTang;
                jt /= denominator;
                jt /= (float)contactCount;
                
                float j = -(1.0f + (rigidObjectA.GetRestitution() + rigidObjectB.GetRestitution()) * 0.5f) * pe2dMath::Dot(relativeVelocity, normal);
                j /= denominator;
                j /= (float)contactCount;

                Vector2 impulseFriction{0.0f, 0.0f};
                
                if(std::abs(jt) <= j * coefficientOfStaticFriction)
                {
                    impulseFriction = jt * tangent;
                }
                else
                {
                    impulseFriction = -j * tangent * coefficientOfDynamicFriction;
                }


                deltaLinearVelocityA += impulseFriction * invMassA;
                deltaAngularVelocityA += pe2dMath::Cross(rA, impulseFriction) * rigidObjectA.GetInvRotationalInertia();
                deltaLinearVelocityB -= impulseFriction * invMassB;
                deltaAngularVelocityB -= pe2dMath::Cross(rB, impulseFriction) * rigidObjectB.GetInvRotationalInertia();
            }   

            rigidObjectA.AddLinearVelocity(deltaLinearVelocityA);
            rigidObjectA.AddAngularVelocity(deltaAngularVelocityA);
            rigidObjectB.AddLinearVelocity(deltaLinearVelocityB);
            rigidObjectB.AddAngularVelocity(deltaAngularVelocityB);
        }
    }

    void PhysicsWorld::MoveObjects(float deltaTime)
    {
        for(auto it = m_Objects.begin(); it != m_Objects.end(); it++)
        {
            RigidObject &object = it->second;
            const Vector2 acceleration = object.GetForce() * object.GetInvMass();
            Vector2 newVel = object.GetLinearVelocity() + acceleration * deltaTime;
            object.Move(object.GetLinearVelocity() * deltaTime + (acceleration * deltaTime * deltaTime * 0.5));
            object.Rotate(object.GetAngularVelocity() * deltaTime * (180.0f / pe2dMath::PI));
            object.SetLinearVelocity(newVel);
            object.SetForce({0.0f, 0.0f});
        }
    }
}