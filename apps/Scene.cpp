#include "Common.h"

int main() {
    auto floor = rt::sphere();
    floor->set_transform(rt::scaling(10, 0.01, 10));
    floor->material().color = rt::color(1, 0.9, 0.9);
    floor->material().specular = 0;

    auto left_wall = rt::sphere();
    left_wall->set_transform(rt::translation(0, 0, 5) *
                             rt::rotation_y(-std::numbers::pi / 4) * rt::rotation_x(std::numbers::pi / 2) *
                             rt::scaling(10, 0.01, 10));
    left_wall->set_material(floor->material());

    auto right_wall = rt::sphere();
    right_wall->set_transform(rt::translation(0, 0, 5) *
                              rt::rotation_y(std::numbers::pi / 4) * rt::rotation_x(std::numbers::pi / 2) *
                              rt::scaling(10, 0.01, 10));
    right_wall->set_material(floor->material());

    auto middle = rt::sphere();
    middle->set_transform(rt::translation(-0.5, 1, 0.5));
    middle->material().color = rt::color(0.1, 1, 0.5);
    middle->material().diffuse = 0.7;
    middle->material().specular = 0.3;

    auto right = rt::sphere();
    right->set_transform(rt::translation(1.5, 0.5, -0.5) * rt::scaling(0.5, 0.5, 0.5));
    right->material().color = rt::color(0.5, 1, 0.1);
    right->material().diffuse = 0.7;
    right->material().specular = 0.3;

    auto left = rt::sphere();
    left->set_transform(rt::translation(-1.5, 0.33, -0.75) * rt::scaling(0.33, 0.33, 0.33));
    left->material().color = rt::color(1, 0.8, 0.1);
    left->material().diffuse = 0.7;
    left->material().specular = 0.3;

    rt::World world{};

    world.add_object(std::move(floor));
    world.add_object(std::move(left_wall));
    world.add_object(std::move(right_wall));
    world.add_object(std::move(middle));
    world.add_object(std::move(right));
    world.add_object(std::move(left));

    world.add_light(rt::point_light(rt::point(-10, 10, -10), rt::color(1, 1, 1)));

    rt::Camera camera{2600, 1800, std::numbers::pi / 3};
    camera.set_transform(rt::view_transform(rt::point(0, 1.5, -5),
                                            rt::point(0, 1, 0),
                                            rt::vector(0, 1, 0)));

    const auto canvas = camera.render(world);

    rt::save_to_file("scene.ppm", canvas);
}
