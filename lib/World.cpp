#include "World.h"

#include <algorithm>

namespace rt {
    std::vector<Intersection> World::intersect(const Ray &ray) const {
        std::vector<Intersection> result;
        for (auto &obj : objects_) {
            auto xs = obj->intersect(ray);
            result.insert(result.end(), xs.begin(), xs.end());
        }
        std::ranges::sort(result, [](auto a, auto b) { return a.t() < b.t(); });
        return result;
    }

    World default_world() {
        constexpr Light light{point(-10, 10, -10), color(1, 1, 1)};

        auto s1 = std::make_unique<Sphere>();
        s1->material().color = color(0.8, 1.0, 0.6);
        s1->material().diffuse = 0.7;
        s1->material().specular = 0.2;

        auto s2 = std::make_unique<Sphere>();
        s2->set_transform(scaling(0.5, 0.5, 0.5));

        World world{};
        world.set_light(light);
        world.add_object(std::move(s1));
        world.add_object(std::move(s2));

        return world;
    }
} // rt