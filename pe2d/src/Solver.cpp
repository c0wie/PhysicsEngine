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
            auto rigidBodyA = std::dynamic_pointer_cast<RigidObject>(objectA);
            auto rigidBodyB = std::dynamic_pointer_cast<RigidObject>(objectB);
            const float massA = rigidBodyA? rigidBodyA->GetMass() : 0.0f;
            const float massB = rigidBodyB? rigidBodyB->GetMass() : 0.0f;
            const Vector2 positionA = objectA->GetPosition();
            const Vector2 positionB = objectB->GetPosition();
            
            Vector2 MTV = points.Normal * points.Depth;
            
            if(MTV.dot(positionA - positionB) < 0.0f)
            {
                MTV *= -1.0f;
            }
            const float totalMass = massA + massB;
            
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
                MTVA = MTV * ( massB / totalMass);
                MTVB = MTV * ( massA / totalMass);
            }
            if(rigidBodyA? false : true)
            {
                objectA->Move(MTVA);
            }
            if(rigidBodyB? false : true)
            {
                objectB->Move(MTVB * -1);
            }
        }
    }

    void ImpulseSolver::Solve(std::vector<Collision> &collisions, float deltaTime)
    {
        for(const auto &collision : collisions)
        {
            auto objectA = collision.GetObjectA();
            auto objectB = collision.GetObjectB();
            const auto &points = collision.GetPoints();
            auto rigidBodyA = std::dynamic_pointer_cast<RigidObject>(objectA);
            auto rigidBodyB = std::dynamic_pointer_cast<RigidObject>(objectB);
            const float massA = rigidBodyA? rigidBodyA->GetMass() : 0.0f;
            const float massB = rigidBodyB? rigidBodyB->GetMass() : 0.0f;
            const Vector2 &positionA = objectA->GetPosition();
            const Vector2 &positionB = objectB->GetPosition();
            
            Vector2 MTV = points.Normal * points.Depth;
            
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
                objectA->Move(MTVA);
                objectB->Move(MTVB * -1);
            }
            else
            {
                objectA->Move(MTV);
            }
        }
    }
}
