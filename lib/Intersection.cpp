#include "Intersection.h"

#include <algorithm>

#include "Sphere.h"

namespace rt {
    std::ostream &operator<<(std::ostream &os, const Intersection &obj) {
        return os
               << "t_: " << obj.t_
               << " object_: " << obj.object_;
    }

    const std::vector<Intersection> &Intersections::data() {
        sort();
        return intersections_;
    }

    std::optional<Intersection> Intersections::hit() {
        sort();
        const auto it = std::ranges::find_if(intersections_, [](auto &x) {
            return x.t() >= 0;
        });
        if (it == intersections_.end()) {
            return {};
        }
        return *it;
    }

    void Intersections::insert(const std::vector<Intersection> &xs) {
        if (!xs.empty()) {
            intersections_.insert(intersections_.end(), xs.begin(), xs.end());
            is_sorted_ = false;
        }
    }

    void Intersections::sort() {
        if (!is_sorted_) {
            std::ranges::sort(intersections_, [](auto &a, auto &b) { return a.t() < b.t(); });
            is_sorted_ = true;
        }
    }

    Computations prepare_computations(const Intersection &intersection, const Ray &ray) {
        Computations comps{};

        comps.t = intersection.t();
        comps.object = intersection.object();

        comps.point = ray.position(comps.t);
        comps.eye_v = -ray.direction();
        comps.normal_v = comps.object->normal_at(comps.point);

        if (comps.normal_v.dot(comps.eye_v) < 0) {
            comps.inside = true;
            comps.normal_v = -comps.normal_v;
        } else {
            comps.inside = false;
        }

        return comps;
    }
} // rt
