set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_executable(my_test test_algo.cpp test_grid.cpp test_rigid_body.cpp
  test_vector2.cpp test_transform.cpp test_collision_points.cpp test_math.cpp test_angle.cpp
)
target_link_libraries(my_test PRIVATE pe2d GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(my_test)