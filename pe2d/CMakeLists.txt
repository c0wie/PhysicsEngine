# Physics Engine 2D
cmake_minimum_required(VERSION 3.28.0)
project(pe2d)

add_library(pe2d STATIC)
target_sources(pe2d PRIVATE "src/Algo.cpp" "src/CircleCollider.cpp" "src/Collision.cpp" "src/CollisionObject.cpp"
                "src/PhysicsWorld.cpp" "src/RigidObject.cpp" "src/Solver.cpp" "src/SquareCollider.cpp")
target_include_directories(pe2d PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")