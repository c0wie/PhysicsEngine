#include "Solver.hpp"

namespace pe2d
{
    void PositionSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        constexpr float slop = 0.01f;
        std::string collisionSide;
        for(int i = 0; i < collisions.size(); i++)
        {
            const Collision &collision = collisions[i];
            const Vector2 &positionA = collision.GetCollisionObjectA()->GetTransform().position;
            const Vector2 &positionB = collision.GetCollisionObjectB()->GetTransform().position;
            const Vector2 &normal = collision.GetPoints().Normal;
            const auto objectA = std::dynamic_pointer_cast<RigidObject>( collision.GetCollisionObjectA() );
            const auto objectB = std::dynamic_pointer_cast<RigidObject>( collision.GetCollisionObjectB() );
           // const float massA = objectA? objectA->GetMass() : 0.0f;
            // const float massB = objectB? objectB->GetMass() : 0.0f;
            //const float totalMass = (massA + massB) != 0.0f? massA + massB : 1.0f;
            
            Vector2 MTV = normal * std::max( collision.GetPoints().Depth - slop, 0.0f );
           // Vector2 MTVA = MTV * (massA / totalMass);
            //Vector2 MTVB = MTV * (massB / totalMass);        
            if(MTV.dot(positionA - positionB) < 0.0f)
            {
                MTV *= -1.0f;
                //MTVB *= -1.0f;
            }
            collision.GetCollisionObjectA()->GetTransform().Move(MTV);
            //collision.GetCollisionObjectB()->GetTransform()->Move(MTVB);
        }
    }

    void ImpulseSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        for(int i = 0; i < collisions.size(); i++)
        {
            
        }
    }
}