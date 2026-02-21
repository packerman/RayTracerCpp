#include "Intersection.h"

#include <algorithm>

#include "Sphere.h"

namespace rt {
    std::optional<Intersection> hit(std::vector<Intersection> &xs) {
        std::ranges::sort(xs, [](auto &a, auto &b) { return a.t() < b.t(); });
        const auto it = std::ranges::find_if(xs, [](auto &x) {
            return x.t() >= 0;
        });
        if (it == xs.end()) {
            return {};
        }
        return *it;
    }

    std::ostream &operator<<(std::ostream &os, const Intersection &obj) {
        return os
               << "t_: " << obj.t_
               << " object_: " << obj.object_;
    }

    Computations prepare_computations(const Intersection &intersection, const Ray &ray) {
        Computations comps{};

        comps.t = intersection.t();
        comps.object = intersection.object();

        comps.point = ray.position(comps.t);
        comps.eye_v = -ray.direction();
        comps.normal_v = comps.object->normal_at(comps.point);

        return comps;
    }
} // rt
