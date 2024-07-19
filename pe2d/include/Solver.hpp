#pragma once

#include <vector>
#include "Collision.hpp"

namespace pe2d
{
    class Solver
    {
    public:
        virtual void Solve(std::vector<Collision> &collisions, float deltaTime) = 0;
    };
    
    class PositionSolver : public Solver
    {
    public:
        void Solve(std::vector<Collision> &collisions, float deltaTime) override final;
    };

    class ImpulseSolver : public Solver
    {
    public:
        void Solve(std::vector<Collision> &collisions, float deltaTime) override final;
    };
}