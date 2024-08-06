#include "Solver.hpp"
namespace pe2d
{
    void PositionSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        for(const auto &collision : collisions)
        {
            auto objectA = collision.GetObjectA();
            auto objectB = collision.GetObjectB();
            const auto &points = collision.GetPoints();
            const Vector2 positionA = objectA->GetPosition();
            const Vector2 positionB = objectB->GetPosition();
            
            Vector2 MTV = points.Normal * points.Depth;
            
            if(MTV.dot(positionA - positionB) < 0.0f)
            {
                MTV *= -1.0f;
            }
            objectA->Move(MTV);
            objectB->Move(MTV * -1.0f);
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
            if(!rigidBodyA && !rigidBodyB)
            {
                PositionSolver solver;
                return solver.Solve(collisions, deltaTime);
            }
            const auto &points = collision.GetPoints();
            const Vector2 positionA = objectA->GetPosition();
            const Vector2 positionB = objectB->GetPosition();
            const float massA = objectA->IsRigid()? rigidBodyA->GetMass() : 0.0f;
            const float massB = objectB->IsRigid()? rigidBodyB->GetMass() : 0.0f;
            
            Vector2 MTV = points.Normal * points.Depth;
            
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
            else
            {
                MTVA = MTV * (massB / totalMass);
                MTVB = MTV * (massA / totalMass);
            }
            objectA->Move(MTVA);
            objectB->Move(MTVB * -1);

            // apply force
            const Vector2 velocityA = objectA->IsRigid()? rigidBodyA->GetVelocity() : Vector2(0.0f, 0.0f); 
            const Vector2 velocityB = objectB->IsRigid()? rigidBodyB->GetVelocity() : Vector2(0.0f, 0.0f); 
            const float dynamicFrictionA = objectA->IsRigid()? rigidBodyA->GetDynamicFriction() : 0.0f;
            const float dynamicFrictionB = objectA->IsRigid()? rigidBodyA->GetDynamicFriction() : 0.0f;
            const float coefficientOfDynamicFriction = (dynamicFrictionA + dynamicFrictionB) / 2.0f;
            const Vector2 relativeVelocity = velocityB - velocityA;
            
            const float velocityAlongNormal = relativeVelocity.dot(points.Normal);
            
            const float inverseMassA = massA > 0.0f ? 1.0f / massA : 0.0f;
            const float inverseMassB = massB > 0.0f ? 1.0f / massB : 0.0f;
            const float impulseScalar = velocityAlongNormal / (inverseMassA + inverseMassB);

            const Vector2 impulse = points.Normal * impulseScalar;
            const Vector2 tangent = (relativeVelocity - points.Normal * velocityAlongNormal).normalized();

            float frictionImpulseScalar = (relativeVelocity.dot(tangent) * -1.0f) / (inverseMassA + inverseMassB);

            const float maxFriction = coefficientOfDynamicFriction * impulseScalar;
            if(std::abs(frictionImpulseScalar) > maxFriction)
            {
                frictionImpulseScalar = maxFriction * (frictionImpulseScalar > 0.0f ? 1.0f : -1.0f);
            }

            const Vector2 frictionImpulse = tangent * frictionImpulseScalar;

            if(rigidBodyA)
            {
                const Vector2 force = velocityA - impulse * inverseMassA;
                rigidBodyA->AddForce(force - frictionImpulse * inverseMassA);
            }
            if(rigidBodyB)
            {
                const Vector2 force = velocityB - impulse * inverseMassB;
                rigidBodyB->AddForce(force - frictionImpulse * inverseMassB);
            }
        }
    }
}
