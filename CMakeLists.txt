cmake_minimum_required(VERSION 3.14)
project(demo)

# Option to build the demo with or without tests
option(BUILD_TESTS "Build tests for the demo" ON)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF)

#SFML
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
set(SFML_BUILD_AUDIO OFF)
FetchContent_MakeAvailable(SFML)

#ImGui
FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.91.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

#ImGui-SFML
set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG v3.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ImGui-SFML)

#Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN_INPUT ${PROJECT_SOURCE_DIR}/pe2d/include ${PROJECT_SOURCE_DIR}/pe2d/src ${PROJECT_SOURCE_DIR}/docs/examples)
  set(DOXYGEN_EXAMPLE_PATH ${PROJECT_SOURCE_DIR}/docs/examples)
  
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/docs/output)
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_PROJECT_NAME "pe2d - c++ 2d Physics Engine")

  doxygen_add_docs(docs
    ${DOXYGEN_INPUT}
    COMMENT "Generating API documentation with Doxygen"
  )
endif()

#tests
if(BUILD_TESTS)
  include(CTest)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  add_subdirectory(tests)
  message(STATUS "Building demo with tests")
else()
  message(STATUS "Building demo without tests")
endif()

#pe2d
add_subdirectory(pe2d)

add_executable(${PROJECT_NAME} 
  demo/demo.cpp demo/visual_world.cpp
)

add_compile_options(
  -Wall
  -Wextra
  -Wpedantic
  -Wshadow
  -Wcast-align
  -Wconversion
  -Wsign-conversion
  -Wnull-dereference
)

target_link_libraries(${PROJECT_NAME} PRIVATE pe2d SFML::Graphics SFML::Window SFML::System ImGui-SFML::ImGui-SFML)
