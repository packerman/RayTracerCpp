#include <Tuple.h>
#include <Transformation.h>
#include <Canvas.h>

#include <numbers>
#include <fstream>

int main() {
    constexpr auto canvas_size = 800;
    constexpr auto radius = static_cast<double>(3 * canvas_size) / 8;
    auto canvas = rt::Canvas{canvas_size, canvas_size};
    constexpr auto color = rt::color(1, 1, 0);

    constexpr auto twelve = rt::point(0, 0, 1);
    const auto hour_rot = rt::rotation_y(std::numbers::pi / 6);
    auto point = twelve;
    for (auto i = 0; i < 12; i++) {
        point = hour_rot * point;
        canvas.write_pixel(
            static_cast<int>(point.x * radius + static_cast<double>(canvas_size) / 2),
            static_cast<int>(point.z * radius + static_cast<double>(canvas_size) / 2),
            color
        );
    }

    std::ofstream outfile("clock.ppm");
    canvas.to_ppm(outfile);
}
