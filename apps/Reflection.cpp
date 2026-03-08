#include "Common.h"

int main() {
    using namespace rt;

    World world{};

    auto floor = plane();
    floor->material().pattern = checkers_pattern(color(0.8, 0.8, 0.8), color(0.2, 0.2, 0.2));
    floor->material().pattern->set_transform(scaling(1.5, 1.5, 1.5)
        .then(rotation_y(std::numbers::pi / 8)));
    floor->material().reflective = 0.3;
    world.add_object(std::move(floor));

    auto sphere1 = sphere();
    sphere1->material().color = color(0.9, 0.2, 0.1);
    sphere1->set_transform(translation(-3, 1, 0));
    sphere1->material().shininess = 5;
    world.add_object(std::move(sphere1));

    auto sphere2 = sphere();
    sphere2->material().color = color(0.1, 0.9, 0.1);
    sphere2->set_transform(translation(3.5, 1, 0));
    sphere2->material().shininess = 5;
    sphere2->material().reflective = 0.15;
    world.add_object(std::move(sphere2));

    world.add_light(point_light(point(-10, 10, -10), color(1, 1, 1)));

    Camera camera{1300, 900, std::numbers::pi / 2};
    camera.set_transform(rt::view_transform(point(0, 1.5, -5),
                                            point(0, 1, 0),
                                            vector(0, 1, 0)));

    const auto canvas = camera.render(world);
    save_to_file("reflection.ppm", canvas);
}
