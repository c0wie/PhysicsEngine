#include "Solver.hpp"
#include "Algo.hpp"
namespace pe2d
{
    void PositionSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        for(size_t i = 0; i < collisions.size(); i++)
        {
            RigidObject &rigidObjectA = collisions[i].ObjectA;
            RigidObject &rigidObjectB = collisions[i].ObjectB;
            const auto &points = collisions[i].Points;
            const float invMassA = rigidObjectA.GetInvMass();
            const float invMassB = rigidObjectB.GetInvMass();

            Vector2 MTV = points.Normal * points.Depth;
            if(Dot(MTV, rigidObjectA.GetPosition() - rigidObjectB.GetPosition()) < 0.0f)
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
        for(size_t i = 0; i < collisions.size(); i++)
        {
            RigidObject &rigidObjectA = collisions[i].ObjectA;
            RigidObject &rigidObjectB = collisions[i].ObjectB;
            const auto &points = collisions[i].Points;
            const float invMassA = rigidObjectA.GetInvMass();
            const float invMassB = rigidObjectB.GetInvMass();

            Vector2 MTV = points.Normal * points.Depth;
            if(Dot(MTV, rigidObjectA.GetPosition() - rigidObjectB.GetPosition()) < 0.0f)
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

            const Vector2 relativeVelocity = rigidObjectB.GetVelocity() - rigidObjectA.GetVelocity();
            const float relativeVelocityAlongNormal = Dot(relativeVelocity, points.Normal);
            
            const float coefficientOfRestitution = (rigidObjectA.GetRestitution() + rigidObjectB.GetRestitution()) * 0.5f;

            float j = -(1.0f + coefficientOfRestitution) * relativeVelocityAlongNormal;
            j /= (invMassA + invMassB);

            const Vector2 impulse = j * points.Normal;

            rigidObjectA.AddVelocity(-1.0f * impulse * invMassA);
            rigidObjectB.AddVelocity(impulse * invMassB);
        }
    }
}
