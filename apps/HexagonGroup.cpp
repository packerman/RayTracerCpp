#include "Common.h"

using namespace std;
using namespace rt;

auto hexagon_corner() {
    auto corner = sphere();
    corner->set_transform(translation(0, 0, -1) *
                          scaling(0.25, 0.25, 0.25));
    return corner;
}

auto hexagon_edge() {
    auto edge = cylinder(0, 1);
    edge->set_transform(translation(0, 0, -1) *
                        rotation_y(-numbers::pi / 6) *
                        rotation_z(-numbers::pi / 2) *
                        scaling(0.25, 1, 0.25));
    return edge;
}

auto hexagon_side() {
    auto side = group();
    side->add_child(hexagon_corner());
    side->add_child(hexagon_edge());
    return side;
}

auto hexagon() {
    auto hex = group();
    for (auto n = 0; n < 6; n++) {
        auto side = hexagon_side();
        side->set_transform(rotation_y(n * numbers::pi / 3));
        hex->add_child(std::move(side));
    }
    return hex;
}

int main() {
    World world;

    world.add_object(hexagon());

    world.add_light(point_light(point(-10, 10, -10),
                                color(1, 1, 1)));

    Camera camera{2600, 1800, std::numbers::pi / 3};
    camera.set_transform(view_transform(point(0, 1.0, -2.5),
                                        point(0, 0, 0),
                                        rt::vector(0, 1, 0)));

    const auto canvas = camera.render(world);

    save_to_file("hexagon_group.ppm", canvas);
}
