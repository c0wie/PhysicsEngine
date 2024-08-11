set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_executable(my_test AlgoUnitTests.cpp)
target_link_libraries(my_test PRIVATE pe2d GTest::gtest_main pe2d)
include(GoogleTest)
gtest_discover_tests(my_test)