#include <fstream>
#include <iostream>

#include <Tuple.h>
#include <Canvas.h>

struct Projectile {
    rt::Point position;
    rt::Vector velocity;
};

struct Environment {
    rt::Vector gravity;
    rt::Vector wind;
};

Projectile tick(const Environment &env, const Projectile &proj) {
    const auto position = proj.position + proj.velocity;
    const auto velocity = proj.velocity + env.gravity + env.wind;
    return Projectile(position, velocity);
}

int main() {
    constexpr auto start = rt::point(0, 1, 0);
    const auto velocity = rt::vector(1, 1.8, 0).normalize() * 11.25;
    auto p = Projectile(start, velocity);

    constexpr auto gravity = rt::vector(0, -0.1, 0);
    constexpr auto wind = rt::vector(-0.01, 0, 0);
    constexpr auto e = Environment(gravity, wind);

    constexpr int height = 550;
    auto c = rt::Canvas(990, height);
    constexpr auto red = rt::color(1, 0, 0);

    size_t t = 0;
    while (p.position.y >= 0) {
        auto x = static_cast<int>(p.position.x);
        auto y = static_cast<int>(height - p.position.y);
        c.write_pixel(x, y, red);
        p = tick(e, p);
        t++;
    }

    std::string filename{"projectile.ppm"};
    if (std::ofstream out{filename}; out.is_open()) {
        c.to_ppm(out);
    } else {
        std::cout << "Failed to open " << filename << std::endl;
    }
}