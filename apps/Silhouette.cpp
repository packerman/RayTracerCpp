#include "Common.h"

using namespace rt;

int main() {
    constexpr auto ray_origin{rt::point(0, 0, -5)};
    constexpr auto wall_size{7.0};

    constexpr std::size_t canvas_pixels{1000};
    constexpr auto pixels_size{wall_size / static_cast<double>(canvas_pixels)};
    constexpr auto half{wall_size / 2};

    Canvas canvas{canvas_pixels, canvas_pixels};
    constexpr auto color{rt::color(1, 0, 0)};
    Sphere shape{};

    for (auto y = 0; y < canvas_pixels; y++) {
        const auto world_y = half - pixels_size * y;

        for (auto x = 0; x < canvas_pixels; x++) {
            constexpr auto wall_z{10};
            const auto world_x = -half + pixels_size * x;

            auto position = point(world_x, world_y, wall_z);

            Ray r{ray_origin, (position - ray_origin).normalize()};

            if (auto xs = shape.intersect(r); hit(xs)) {
                canvas.write_pixel(x, y, color);
            }
        }
    }

    save_to_file("silhouette.ppm", canvas);
}
