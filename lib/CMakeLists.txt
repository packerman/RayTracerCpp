add_library(ray_tracer)

target_sources(ray_tracer
        PRIVATE
        Tuple.h
        Tuple.cpp
        Canvas.h
        Canvas.cpp
        Matrix.h
        Matrix.cpp
)

target_include_directories(ray_tracer
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)