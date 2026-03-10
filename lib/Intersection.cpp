#include "Intersection.h"

#include <algorithm>
#include <list>

#include "Shape.h"

namespace rt {
    std::ostream &operator<<(std::ostream &os, const Intersection &obj) {
        return os
               << "t_: " << obj.t_
               << " object_: " << obj.object_;
    }

    const Intersection &Intersections::operator[](const std::size_t i) {
        ensure_sorted();
        return intersections_[i];
    }

    const std::vector<Intersection> &Intersections::data() {
        ensure_sorted();
        return intersections_;
    }

    std::optional<Intersection> Intersections::hit() {
        ensure_sorted();
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

    void Intersections::ensure_sorted() {
        if (!is_sorted_) {
            std::ranges::sort(intersections_, [](auto &a, auto &b) { return a.t() < b.t(); });
            is_sorted_ = true;
        }
    }

    Computations prepare_computations(const Intersection &intersection, const Ray &ray) {
        Intersections xs{intersection};
        return prepare_computations(intersection, ray, xs);
    }

    double schlick(const Computations &comps) {
        auto cos = dot(comps.eye_v, comps.normal_v);
        if (comps.n1 > comps.n2) {
            const auto n = comps.n1 / comps.n2;
            const auto sin2_t = n * n * (1 - cos * cos);
            if (sin2_t > 1) {
                return 1;
            }
            const auto cos_t = std::sqrt(1 - sin2_t);
            cos = cos_t;
        }
        const auto r0 = std::pow((comps.n1 - comps.n2) / (comps.n1 + comps.n2), 2);
        return r0 + (1 - r0) * std::pow(1 - cos, 5);
    }

    void compute_refractive_indices(const Intersection &hit, Computations &comps, const std::vector<Intersection> &xs);

    Computations prepare_computations(const Intersection &intersection, const Ray &ray, Intersections &xs) {
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

        comps.over_point = comps.point + comps.normal_v * shadow_epsilon;
        comps.under_point = comps.point - comps.normal_v * shadow_epsilon;

        comps.reflect_v = ray.direction().reflect(comps.normal_v);

        compute_refractive_indices(intersection, comps, xs.data());

        return comps;
    }

    void compute_refractive_indices(const Intersection &hit, Computations &comps,
                                    const std::vector<Intersection> &xs) {
        std::list<Shape *> containers{};

        for (auto &i: xs) {
            if (i == hit) {
                if (containers.empty()) {
                    comps.n1 = 1;
                } else {
                    comps.n1 = containers.back()->material().refractive_index;
                }
            }

            if (auto it = std::ranges::find(containers, i.object()); it != containers.end()) {
                containers.erase(it);
            } else {
                containers.push_back(i.object());
            }

            if (i == hit) {
                if (containers.empty()) {
                    comps.n2 = 1;
                } else {
                    comps.n2 = containers.back()->material().refractive_index;
                }
                break;
            }
        }
    }
}
