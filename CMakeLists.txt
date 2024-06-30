cmake_minimum_required(VERSION 3.28.0)
project(DEMO LANGUAGES CXX VERSION 1.0)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE( SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
FETCHCONTENT_MAKEAVAILABLE(SFML)

add_subdirectory(pe2d)
add_subdirectory(ImGui-SFML)

add_executable(${PROJECT_NAME} 
    Demo.cpp
    tests/src/TestCollision.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics pe2d ImGui-SFML)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/tests/include")
