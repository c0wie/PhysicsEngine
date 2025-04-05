/// @file
#pragma once

// local
#include "collision.hpp"

// std
#include <vector>

namespace pe2d {

/// @brief Function correcting position of RigidBodies
void PositionSolver(std::vector<Collision> &collisions);

/*
    Function responsible for calculating collisions between rigidObjects.
    The solver take into account the torque, their velocity, bounciness, and
   both static and dynamic friction.
*/
void ImpulseSolver(std::vector<Collision> &collisions);
} // namespace pe2d