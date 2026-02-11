
add_executable(projectile)

target_sources(projectile
        PRIVATE
        Projectile.cpp
)

target_link_libraries(projectile
        PRIVATE
        ray_tracer
)