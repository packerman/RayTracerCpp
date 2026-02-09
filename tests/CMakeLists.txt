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
)

target_link_libraries(test_lib
        PRIVATE
        lib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_lib)