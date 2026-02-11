#include <iostream>

#include <Tuple.h>

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
    auto p = Projectile(rt::Tuple::point(0, 1, 0), rt::Tuple::vector(1, 1, 0).normalize());
    const auto e = Environment(rt::Tuple::vector(0, -0.1, 0), rt::Tuple::vector(-0.01, 0, 0));
    size_t t = 0;
    while (p.position.y >= 0) {
        std::cout << t << ": " << p.position << "\n";
        p = tick(e, p);
        t++;
    }
}