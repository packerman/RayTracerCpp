cmake_minimum_required(VERSION 3.31)
project(RayTracer)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_subdirectory(lib)
add_subdirectory(apps)
add_subdirectory(tests)