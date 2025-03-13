#pragma once

// local
#include "collision.hpp"

// std
#include <vector>

namespace pe2d {
/*
    Function responsible for calculating collisions between rigidObjects.
    The solver only pulls them apart to prevent penetration.
*/
void PositionSolver(std::vector<Collision> &collisions, float delta_time);

/*
    Function responsible for calculating collisions between rigidObjects.
    The solver take into account the torque, their velocity, and bounciness.
*/
void ImpulseSolverWithoutFriction(std::vector<Collision> &collisions,
                                  float delta_time);

/*
    Function responsible for calculating collisions between rigidObjects.
    The solver take into account the torque, their velocity, bounciness, and
   both static and dynamic friction.
*/
void ImpulseSolverWithFriction(std::vector<Collision> &collisions,
                               float delta_time);
} // namespace pe2d