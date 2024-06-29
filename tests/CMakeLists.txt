cmake_minimum_required(VERSION 3.28.0)
project(tests)

add_library(tests STATIC)
target_sources(tests PRIVATE "src/Test.cpp")
target_include_directories(tests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")