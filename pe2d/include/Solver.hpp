#pragma once

#include <vector>
#include "Collision.hpp"

namespace pe2d
{
    // virtual class that solves collsions
    class Solver
    {
    public:
        virtual void Solve(std::vector<Collision> &collisions, float deltaTime) = 0;
    };
    
    // class that do position correction on objects
    class PositionSolver : public Solver
    {
    public:
        void Solve(std::vector<Collision> &collisions, float deltaTime) override final;
    };

    // class that simulate real world forces and apply them to push objects away from each other
    class ImpulseSolver : public Solver
    {
    public:
        void Solve(std::vector<Collision> &collisions, float deltaTime) override final;
    };
}