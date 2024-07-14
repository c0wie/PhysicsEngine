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
            
            auto objA = std::dynamic_pointer_cast<RigidObject>( collision.GetObjectA() );
            auto objB = std::dynamic_pointer_cast<RigidObject>( collision.GetObjectB() );
            const float massA = objA? objA->GetMass() : 0.0f;
            const float massB = objB? objB->GetMass() : 0.0f;
            const Vector2 &positionA = collision.GetObjectA()->GetPosition();
            const Vector2 &positionB = collision.GetObjectB()->GetPosition();
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
                collision.GetObjectA()->Move(MTVA);
                collision.GetObjectB()->Move(MTVB * -1);
            }
            else
            {
                collision.GetObjectA()->Move(MTV);
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