#include "Common.h"

using namespace rt;

int main() {
    constexpr auto ray_origin{rt::point(0, 0, -5)};
    constexpr auto wall_size{7.0};

    constexpr std::size_t canvas_pixels{1000};
    constexpr auto pixels_size{wall_size / static_cast<double>(canvas_pixels)};
    constexpr auto half{wall_size / 2};

    Canvas canvas{canvas_pixels, canvas_pixels};
    Sphere shape{};
    shape.set_material(Material{color(1, 0.2, 1)});

    Light light{point(-10, 10, -10), color(1, 1, 1)};

    for (auto y = 0; y < canvas_pixels; y++) {
        const auto world_y = half - pixels_size * y;

        for (auto x = 0; x < canvas_pixels; x++) {
            constexpr auto wall_z{10};
            const auto world_x = -half + pixels_size * x;

            auto position = point(world_x, world_y, wall_z);

            Ray ray{ray_origin, (position - ray_origin).normalize()};

            auto xs = shape.intersect(ray);
            if (auto hit = rt::hit(xs)) {
                auto point = ray.position(hit->t());
                auto normal = hit->object()->normal_at(point);
                auto eye = -ray.direction();
                auto color = hit->object()->material().lighting(light, point, eye, normal);
                canvas.write_pixel(x, y, color);
            }
        }
    }

    save_to_file("silhouette.ppm", canvas);
}
