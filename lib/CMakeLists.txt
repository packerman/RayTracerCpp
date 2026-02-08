add_library(lib)

target_sources(lib
        PRIVATE
        tuple.h
        tuple.cpp
)

target_include_directories(lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)