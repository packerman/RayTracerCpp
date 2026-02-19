cmake_minimum_required(VERSION 4.1)
project(RayTracer)

set(CMAKE_CXX_STANDARD 23)

enable_testing()

add_subdirectory(lib)
add_subdirectory(apps)
add_subdirectory(tests)