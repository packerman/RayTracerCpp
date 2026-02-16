
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