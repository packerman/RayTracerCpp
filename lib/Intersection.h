#pragma once

#include <optional>
#include <ostream>
#include <vector>

#include "Ray.h"
#include "Tuple.h"

namespace rt {
    class Sphere;

    class Intersection {
    public:
        Intersection() = default;

        Intersection(const double t, Sphere *object) : t_{t}, object_{object} {
        }

        [[nodiscard]] double t() const {
            return t_;
        }

        [[nodiscard]] Sphere *object() const {
            return object_;
        }

        friend bool operator==(const Intersection &lhs, const Intersection &rhs) {
            return std::tie(lhs.t_, lhs.object_) == std::tie(rhs.t_, rhs.object_);
        }

        friend bool operator!=(const Intersection &lhs, const Intersection &rhs) {
            return !(lhs == rhs);
        }

        friend std::ostream & operator<<(std::ostream &os, const Intersection &obj);

    private:
        double t_{};
        Sphere *object_{};
    };

    inline std::vector<Intersection> intersections(const std::initializer_list<Intersection> list) {
        return {list};
    }

    std::optional<Intersection> hit(std::vector<Intersection> &xs);

    struct Computations {
        double t;
        Sphere *object;
        Point point;
        Vector eye_v;
        Vector normal_v;
    };

    Computations prepare_computations(const Intersection &intersection, const Ray& ray);
} // rt
