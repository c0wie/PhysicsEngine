# Physics Engine 2D
cmake_minimum_required(VERSION 3.14)
project(pe2d)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(pe2d STATIC)

target_sources(pe2d PRIVATE 
  src/algo.cpp src/collider.cpp
  src/physics_world.cpp src/grid.cpp
  src/rigid_body.cpp src/solver.cpp src/algo.cpp src/collision_world.cpp)

target_include_directories(pe2d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
