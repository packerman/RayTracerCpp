add_library(ray_tracer)

target_sources(ray_tracer
        PRIVATE
        Tuple.h
        Canvas.h
        Canvas.cpp
        Matrix.h
        Transformation.h
        Ray.h
        Sphere.h
        Sphere.cpp
        Intersection.h
        Intersection.cpp
        Material.h
        Material.cpp
        World.h
        World.cpp
)

target_include_directories(ray_tracer
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)