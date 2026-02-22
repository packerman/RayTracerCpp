
add_executable(projectile)
target_sources(projectile
        PRIVATE
        Projectile.cpp
)
target_link_libraries(projectile
        PRIVATE
        ray_tracer
)

add_executable(clock)
target_sources(clock
        PRIVATE
        Clock.cpp)
target_link_libraries(clock
        PRIVATE
        ray_tracer
)

add_executable(silhouette)
target_sources(silhouette
        PRIVATE
        Common.h
        Silhouette.cpp)
target_link_libraries(silhouette
        PRIVATE
        ray_tracer
)

add_executable(scene)
target_sources(scene
        PRIVATE
        Common.h
        Scene.cpp)
target_link_libraries(scene
        PRIVATE
        ray_tracer
)