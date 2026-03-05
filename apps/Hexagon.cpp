#include "Common.h"

using namespace rt;

int main() {
    auto top = plane();
    top->set_transform(
        rotation_x(std::numbers::pi)
        .then(translation(0, 10, 0)));
    top->material().color = color(1, 1, 0);
    auto bottom = plane();
    bottom->set_transform(translation(0, -10, 0));
    bottom->material().color = color(1, 1, 0);

    auto top_left = plane();
    top_left->material().color = color(0, 1, 0);
    top_left->set_transform(
        rotation_z(std::numbers::pi / 3)
        .then(translation(-10, 10, 0)));

    auto bottom_left = plane();
    bottom_left->material().color = color(0, 0, 1);
    bottom_left->set_transform(
        rotation_z(2*std::numbers::pi / 3)
        .then(translation(-10, -10, 0)));

    auto top_right = plane();
    top_right->material().color = color(0, 1, 0);
    top_right->set_transform(
        rotation_z(-std::numbers::pi / 3)
        .then(translation(10, 10, 0)));

    auto bottom_right = plane();
    bottom_right->material().color = color(0, 0, 1);
    bottom_right->set_transform(
        rotation_z(-2*std::numbers::pi / 3)
        .then(translation(10, -10, 0)));

    auto wall = plane();
    wall->material().color = color(0.5, 0, 0);
    wall->material().specular = 0.3;
    wall->set_transform(rotation_x(std::numbers::pi / 2)
        .then(translation(0, 0, -125)));

    World world{};
    world.add_object(std::move(top));
    world.add_object(std::move(bottom));
    world.add_object(std::move(top_left));
    world.add_object(std::move(bottom_left));
    world.add_object(std::move(wall));
    world.add_object(std::move(top_right));
    world.add_object(std::move(bottom_right));

    world.add_light(point_light(point(0, 0, 0),
                                color(1, 1, 1)));

    const Camera camera{2600, 1800, std::numbers::pi / 3};

    const auto canvas = camera.render(world);

    save_to_file("hexagon.ppm", canvas);
}
