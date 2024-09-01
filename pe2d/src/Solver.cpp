#include "Solver.hpp"
#include "Algo.hpp"
namespace pe2d
{
    void PositionSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        for(size_t i = 0; i < collisions.size(); i++)
        {
            RigidObject &rigidObjectA = collisions[i].GetObjectA();
            RigidObject &rigidObjectB = collisions[i].GetObjectB();
            const auto &points = collisions[i].GetCollisionPoints();
            const Vector2 normal = points.Normal;
            
            Vector2 MTV = normal * points.Depth;
            if(pe2dMath::Dot(MTV, rigidObjectA.GetPosition() - rigidObjectB.GetPosition()) < 0.0f)
            {
                MTV *= -1.0f;
            }
            
            if (rigidObjectA.IsStatic())
            {
                rigidObjectB.Move(-1.0f * MTV);
            }
            else if (rigidObjectB.IsStatic())
            {
                rigidObjectA.Move(MTV);
            }
            else
            {
                rigidObjectA.Move(MTV / 2.0f);
                rigidObjectB.Move(MTV / -2.0f);
            }
        }
    }

    void ImpulseSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        for(auto &collision : collisions)
        {
            RigidObject &rigidObjectA = collision.GetObjectA();
            RigidObject &rigidObjectB = collision.GetObjectB();
            const CollisionPoints &points = collision.GetCollisionPoints();
            const float invMassA = rigidObjectA.GetInvMass();
            const float invMassB = rigidObjectB.GetInvMass();
            const float invInertiaA = rigidObjectA.GetInvRotationalInertia();
            const float invInertiaB = rigidObjectB.GetInvRotationalInertia();
            const Vector2 normal = points.Normal;

            Vector2 MTV = normal * points.Depth;
            if(pe2dMath::Dot(MTV, rigidObjectA.GetPosition() - rigidObjectB.GetPosition()) < 0.0f)
            {
                MTV *= -1.0f;
            }
            if (rigidObjectA.IsStatic())
            {
                rigidObjectB.Move(-1.0f * MTV);
            }
            else if (rigidObjectB.IsStatic())
            {
                rigidObjectA.Move(MTV);
            }
            else
            {
                rigidObjectA.Move(MTV / 2.0f);
                rigidObjectB.Move(MTV / -2.0f);
            }

            const int contactCount = points.ContactCount;
            const float coefficientOfRestitution = (rigidObjectA.GetRestitution() + rigidObjectB.GetRestitution()) * 0.5f;
            const Vector2 contactList[2] = { points.ContactPoint1, points.ContactPoint2 };

            Vector2 deltaLinearVelocityA{0.0f, 0.0f};
            float deltaAngularVelocityA{0.0f};
            Vector2 deltaLinearVelocityB{0.0f, 0.0f};
            float deltaAngularVelocityB{0.0f};

            for(int i = 0; i < contactCount; i++)
            {
                // vector pointing from center of mass of the objects to the contact points
                const Vector2 rA = contactList[i] - rigidObjectA.GetPosition();
                const Vector2 rB = contactList[i] - rigidObjectB.GetPosition();
                
                const Vector2 rAperp = pe2dMath::Perp(rA);
                const Vector2 rBperp = pe2dMath::Perp(rB);

                const Vector2 relativeVelocity = (rigidObjectA.GetLinearVelocity() + rAperp * rigidObjectA.GetAngularVelocity()) 
                                            - (rigidObjectB.GetLinearVelocity() + rBperp  * rigidObjectB.GetAngularVelocity()); 

                const float relativeVelocityAlongNormal = pe2dMath::Dot(relativeVelocity, normal);

                const float rAperpNormal = pe2dMath::Dot(rAperp, normal);
                const float rBperpNormal = pe2dMath::Dot(rBperp, normal);

                const float denominator = invMassA + invMassB 
                    + (rAperpNormal * rAperpNormal) * invInertiaA
                    + (rBperpNormal * rBperpNormal) * invInertiaB;

                float j = -(1.0f + coefficientOfRestitution) * relativeVelocityAlongNormal;
                j /= denominator;
                j /= (float)contactCount;
                const Vector2 impulse = j * normal;

                deltaLinearVelocityA += impulse * invMassA;
                deltaAngularVelocityA += pe2dMath::Cross(rA, impulse) * invInertiaA;
                deltaLinearVelocityB -= impulse * invMassB;
                deltaAngularVelocityB -= pe2dMath::Cross(rB, impulse) * invInertiaB;
            }   
            
            rigidObjectA.AddLinearVelocity(deltaLinearVelocityA);
            rigidObjectA.AddAngularVelocity(deltaAngularVelocityA);
            rigidObjectB.AddLinearVelocity(deltaLinearVelocityB);
            rigidObjectB.AddAngularVelocity(deltaAngularVelocityB);
        }
    }
}
