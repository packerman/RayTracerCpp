include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_lib)

target_sources(test_lib
        PRIVATE
        Common.h
        Common.cpp
        Tuple_test.cpp
        Canvas_test.cpp
        Matrix_test.cpp
        Transformation_test.cpp
        Ray_test.cpp
        Shape_test.h
        Shape_test.cpp
        Intersection_test.cpp
        Lighting_test.cpp
        World_test.cpp
        Camera_test.cpp
        Pattern_test.h
        Pattern_test.cpp
        Group_test.cpp
        Bounds_test.cpp
)

target_link_libraries(test_lib
        PRIVATE
        ray_tracer
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_lib)