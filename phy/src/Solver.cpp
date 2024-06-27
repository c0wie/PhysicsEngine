#include "../headers/Solver.hpp"

namespace phy
{
    void PositionSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        constexpr float slop = 0.01f;
        for(int i = 0; i < collisions.size(); i++)
        {
            Collision &collision = collisions[i];
            const Vector2 &positionA = collision.GetCollisionObjectA()->GetTransform()->position;
            const Vector2 &positionB = collision.GetCollisionObjectB()->GetTransform()->position;
            Vector2 MTV = collision.GetPoints().Normal * std::max(collision.GetPoints().Depth - slop, 0.0f);

            if(MTV.dot(positionA - positionB) < 0.0f)
            {
                MTV *= -1.0f;
            }
            collision.GetCollisionObjectA()->GetTransform()->Move(MTV);
        }
    }

    void ImpulseSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        for(int i = 0; i < collisions.size(); i++)
        {
            
        }
    }
}