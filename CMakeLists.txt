cmake_minimum_required(VERSION 3.28)
project(DEMO)

include(CTest)
include(FetchContent)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF)


FetchContent_Declare( 
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
    
    
enable_testing()

add_subdirectory(pe2d)
add_subdirectory(ImGui-SFML)
add_subdirectory(Tests)

add_executable(${PROJECT_NAME} 
    Sample/src/Test.cpp
    Sample/src/CollisionArenaTest.cpp
    Demo.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Samples/include")
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics pe2d ImGui-SFML)
