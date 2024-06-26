#pragma once

#include <vector>
#include "Collision.hpp"

namespace phy
{
    class Solver
    {
    public:
        virtual void Solve(std::vector<Collision> &collisions, float deltaTime) = 0;
    };
    class PositionSolver : public Solver
    {
        void Solve(std::vector<Collision> &collisions, float deltaTime) override final;
    };
}