#include "World.h"

#include <algorithm>

namespace rt {
    Intersections World::intersect(const Ray &ray) const {
        Intersections result;
        for (auto &obj: objects_) {
            auto xs = obj->intersect(ray);
            result.insert(xs);
        }
        return result;
    }

    Color World::shade_hit(const Computations &comps, const int remaining) const {
        auto result = color(0, 0, 0);
        for (auto &light: lights_) {
            const auto shadowed = is_shadowed(comps.over_point, *light);
            auto surface = comps.object->material()
                    .lighting(*comps.object, *light, comps.over_point, comps.eye_v, comps.normal_v, shadowed);
            auto reflected = reflected_color(comps, remaining);
            auto refracted = refracted_color(comps, remaining);
            result += surface + reflected + refracted;
        }
        return result;
    }

    Color World::color_at(const Ray &ray, const int remaining) const {
        auto xs = intersect(ray);
        const auto hit = xs.hit();
        if (!hit) {
            return black;
        }
        const auto comps = prepare_computations(hit.value(), ray);
        return shade_hit(comps, remaining);
    }

    bool World::is_shadowed(const Point &point, const Light &light) const {
        const auto v = light.position - point;
        const auto distance = v.magnitude();
        const auto direction = v.normalize();

        const Ray r{point, direction};
        auto intersections = intersect(r);

        const auto h = intersections.hit();
        return h && h->t() < distance;
    }

    Color World::reflected_color(const Computations &comps, const int remaining) const {
        if (remaining <= 0) {
            return black;
        }
        if (comps.object->material().reflective == 0) {
            return black;
        }
        const Ray reflect_ray{comps.over_point, comps.reflect_v};
        const auto color = color_at(reflect_ray, remaining - 1);

        return color * comps.object->material().reflective;
    }

    Color World::refracted_color(const Computations &comps, int remaining) const {
        if (remaining <= 0) {
            return black;
        }
        if (comps.object->material().transparency == 0) {
            return black;
        }
        const auto n_ratio = comps.n1 / comps.n2;
        const auto cos_i = dot(comps.eye_v, comps.normal_v);
        const auto sin2_t = n_ratio * n_ratio * (1 - cos_i * cos_i);
        if (sin2_t > 1) {
            return black;
        }
        const auto cos_t = std::sqrt(1 - sin2_t);
        const auto direction = comps.normal_v * (n_ratio * cos_i - cos_t) - comps.eye_v * n_ratio;
        const Ray refract_ray{comps.under_point, direction};
        const auto color = color_at(refract_ray, remaining - 1) * comps.object->material().transparency;
        return color;
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
}
