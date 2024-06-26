#include "../headers/Solver.hpp"

namespace phy
{
    void PositionSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        constexpr float slop = 0.01f;
        for(int i = 0; i < collisions.size(); i++)
        {
            Collision &collision = collisions[i];
            Vector2 MTV = collision.GetPoints().Normal * std::max(collision.GetPoints().Depth - slop, 0.0f);
            if(MTV.dot(collision.GetCollisionObjectA()->GetTransform()->GetPosition() - collision.GetCollisionObjectB()->GetTransform()->GetPosition()) < 0.0f)
            {
                MTV *= -1.0f;
            }
            collision.GetCollisionObjectA()->GetTransform()->Move(MTV);
        }
    }
}