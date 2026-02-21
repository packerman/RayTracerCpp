#include "World.h"

#include <algorithm>

namespace rt {
    std::vector<Intersection> World::intersect(const Ray &ray) const {
        std::vector<Intersection> result;
        for (auto &obj: objects_) {
            auto xs = obj->intersect(ray);
            result.insert(result.end(), xs.begin(), xs.end());
        }
        std::ranges::sort(result, [](auto a, auto b) { return a.t() < b.t(); });
        return result;
    }

    Color World::shade_hit(const Computations &comps) const {
        Color result{color(0, 0, 0)};
        for (auto &light: lights_) {
            result += comps.object->material()
                    .lighting(*light, comps.point, comps.eye_v, comps.normal_v);
        }
        return result;
    }

    Color World::color_at(const Ray &ray) const {
        auto xs = intersect(ray);
        const auto hit = rt::hit(xs);
        if (!hit) {
            return black;
        }
        const auto comps = prepare_computations(hit.value(), ray);
        return shade_hit(comps);
    }

    World default_world() {
        auto light = std::make_unique<Light>(point(-10, 10, -10), color(1, 1, 1));

        auto s1 = sphere();
        s1->material().color = color(0.8, 1.0, 0.6);
        s1->material().diffuse = 0.7;
        s1->material().specular = 0.2;

        auto s2 = sphere();
        s2->set_transform(scaling(0.5, 0.5, 0.5));

        World world{};
        world.add_light(std::move(light));
        world.add_object(std::move(s1));
        world.add_object(std::move(s2));

        return world;
    }
} // rt
