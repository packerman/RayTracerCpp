add_library(lib)

target_sources(lib
        PRIVATE
        Tuple.h
        Tuple.cpp
)

target_include_directories(lib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)