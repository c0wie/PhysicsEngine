#include "Solver.hpp"
namespace pe2d
{
    void PositionSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        for(size_t i = 0; i < collisions.size(); i++)
        {
            std::shared_ptr<CollisionObject> objectA = collisions[i].GetObjectA();
            std::shared_ptr<CollisionObject> objectB = collisions[i].GetObjectB();
            std::shared_ptr<RigidObject> rigidBodyA = objectA->IsRigid()? std::static_pointer_cast<RigidObject>(objectA) : nullptr;
            std::shared_ptr<RigidObject> rigidBodyB = objectB->IsRigid()? std::static_pointer_cast<RigidObject>(objectB) : nullptr;
            const auto &points = collisions[i].GetPoints();
            
            const Vector2 relativeVelocity = rigidBodyB->GetVelocity() - rigidBodyA->GetVelocity();
            const float relativeVelocityAlongNormal = relativeVelocity.dot(points.Normal);
            
            if(relativeVelocityAlongNormal > 0.0f)
            {
                return;
            }

            float j = -relativeVelocityAlongNormal;
            j /= (rigidBodyA->GetInvMass() + rigidBodyB->GetInvMass());

            const Vector2 impulse = j * points.Normal;

            rigidBodyA->AddVelocity(-1.0f * impulse * rigidBodyA->GetInvMass());
            rigidBodyB->AddVelocity(impulse * rigidBodyB->GetInvMass());
        }
    }

    void ImpulseSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        for(const auto &collision : collisions)
        {
            std::shared_ptr<CollisionObject> objectA = collision.GetObjectA();
            std::shared_ptr<CollisionObject> objectB = collision.GetObjectB();
            std::shared_ptr<RigidObject> rigidBodyA = objectA->IsRigid()? std::static_pointer_cast<RigidObject>(objectA) : nullptr;
            std::shared_ptr<RigidObject> rigidBodyB = objectB->IsRigid()? std::static_pointer_cast<RigidObject>(objectB) : nullptr;
            const auto &points = collision.GetPoints();
            const Vector2 positionA = objectA->GetPosition();
            const Vector2 positionB = objectB->GetPosition();
            const float massA = objectA->IsRigid()? rigidBodyA->GetMass() : 0.0f;
            const float massB = objectB->IsRigid()? rigidBodyB->GetMass() : 0.0f;
            
            Vector2 MTV = points.Normal * points.Depth;
            
            if(points.Depth == 0.0f)
            {
                return;
            }
            if(MTV.dot(positionA - positionB) < 0.0f)
            {
                MTV *= -1.0f;
            }

            const float totalMass = massA + massB > 0.0f? (massA + massB) : 1.0f;
            Vector2 MTVB, MTVA;
            if(massA == 0.0f)
            {
                MTVB = MTV;
                MTVA = Vector2{};
            }
            else if(massB == 0.0f)
            {
                MTVA = MTV;
                MTVB = Vector2{};
            }
            else if(massA != 0.0f && massB != 0.0f)
            {
                MTVA = MTV * (massB / totalMass);
                MTVB = MTV * (massA / totalMass);
            }
            /*if(rigidBodyA)
            {   
                const float normalForceScalar = points.Normal.dot(rigidBodyA->GetGravity() * rigidBodyA->GetMass() * -1);
                const Vector2 normalForce = points.Normal * normalForceScalar;
                rigidBodyA->AddForce(normalForce);
            }*/
            objectA->Move(MTVA);
            /*if(rigidBodyB)
            {
                const float normalForceScalar = points.Normal.dot(rigidBodyB->GetGravity() * rigidBodyB->GetMass());
                const Vector2 normalForce = points.Normal * normalForceScalar;
                rigidBodyB->AddForce(normalForce);
            }*/
            objectB->Move(MTVB * -1);
        }
    }
}
