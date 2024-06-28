#include "../headers/Solver.hpp"

namespace phy
{
    void PositionSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        constexpr float slop = 0.01f;
        for(int i = 0; i < collisions.size(); i++)
        {
            const Collision &collision = collisions[i];
            const auto &objectA = collision.GetCollisionObjectA();
            const auto &objectB = collision.GetCollisionObjectB();
            const Vector2 &positionA = objectA->GetTransform()->position * objectA->GetTransform()->scale;
            const Vector2 &positionB = collision.GetCollisionObjectB()->GetTransform()->position;
            Vector2 MTV = collision.GetPoints().Normal * std::max( collision.GetPoints().Depth - slop, 0.0f );        

            if(MTV.dot(positionA - positionB) < 0.0f)
            {
                MTV *= -1.0f;
            }
            collision.GetCollisionObjectA()->GetTransform()->Move(MTV);

            std::shared_ptr<RigidObject> rigidObject = std::dynamic_pointer_cast<RigidObject>(objectA); 
            if(rigidObject != nullptr)
            {
                std::shared_ptr<SquareCollider> floor = std::dynamic_pointer_cast<SquareCollider>(objectB->GetCollider());
                if(floor != nullptr)
                {
                    rigidObject->SetVelocity(Vector2{rigidObject->GetVelocity().x, 0.0f});
                }
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