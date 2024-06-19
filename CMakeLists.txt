cmake_minimum_required(VERSION 3.5.0)
project(physicsEngine VERSION 1.0 LANGUAGES C CXX)


#setup SFML
include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE(SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 2.6.x
)
FETCHCONTENT_MAKEAVAILABLE(SFML)

add_executable(physicsEngine 
    Demo.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR} /phy)

target_link_libraries(${PROJECT_NAME} sfml-graphics)