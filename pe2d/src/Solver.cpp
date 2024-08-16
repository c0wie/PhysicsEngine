#include "Solver.hpp"
namespace pe2d
{
    void PositionSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        for(size_t i = 0; i < collisions.size(); i++)
        {
            RigidObject &rigidObjectA = collisions[i].GetObjectA();
            RigidObject &rigidObjectB = collisions[i].GetObjectB();
            const auto &points = collisions[i].GetPoints();
            const float invMassA = rigidObjectA.GetInvMass();
            const float invMassB = rigidObjectB.GetInvMass();

            const Vector2 relativeVelocity = rigidObjectB.GetVelocity() - rigidObjectA.GetVelocity();
            const float relativeVelocityAlongNormal = relativeVelocity.dot(points.Normal);
            
            if(relativeVelocityAlongNormal > 0.0f)
            {
                return;
            }

            float j = -relativeVelocityAlongNormal;
            j /= (invMassA + invMassB);

            const Vector2 impulse = j * points.Normal;

            rigidObjectA.AddVelocity(-1.0f * impulse * invMassA);
            rigidObjectB.AddVelocity(impulse * invMassB);
        }
    }

    void ImpulseSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
    }
}
