#include "Solver.hpp"

namespace pe2d
{
    void PositionSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        for(const auto &collision : collisions)
        {
            const auto &points = collision.GetPoints();
            if(points.Depth <= 0.0f)
            {
                continue;
            }

            auto objA = std::dynamic_pointer_cast<RigidObject>( collision.GetCollisionObjectA() );
            auto objB = std::dynamic_pointer_cast<RigidObject>( collision.GetCollisionObjectB() );
            const float massA = objA? objA->GetMass() : 0.0f;
            const float massB = objB? objB->GetMass() : 0.0f;
            const Vector2 &positionA = collision.GetCollisionObjectA()->GetTransform().position;
            const Vector2 &positionB = collision.GetCollisionObjectB()->GetTransform().position;
            const Vector2 &normal = points.Normal;
            
            Vector2 MTV = normal * points.Depth;
            
            if(MTV.dot(positionA - positionB) < 0.0f)
            {
                MTV *= -1.0f;
            }
            const float totalMass = massA + massB;
            if(totalMass != 0.0f)
            {
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
                collision.GetCollisionObjectA()->Move(MTVA);
                collision.GetCollisionObjectB()->Move(MTVB * -1);
            }
            else
            {
                collision.GetCollisionObjectA()->Move(MTV);
            }
        }
    }

    void ImpulseSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        for(int i = 0; i < collisions.size(); i++)
        {
            
        }
    }
}