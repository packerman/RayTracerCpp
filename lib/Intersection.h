#pragma once

#include <optional>
#include <ostream>
#include <vector>

#include "Ray.h"
#include "Tuple.h"

namespace rt {
    class Shape;

    class Intersection {
    public:
        Intersection() = default;

        Intersection(const double t, Shape *object) : t_{t}, object_{object} {
        }

        [[nodiscard]] double t() const {
            return t_;
        }

        [[nodiscard]] Shape *object() const {
            return object_;
        }

        friend bool operator==(const Intersection &lhs, const Intersection &rhs) {
            return std::tie(lhs.t_, lhs.object_) == std::tie(rhs.t_, rhs.object_);
        }

        friend bool operator!=(const Intersection &lhs, const Intersection &rhs) {
            return !(lhs == rhs);
        }

        friend std::ostream &operator<<(std::ostream &os, const Intersection &obj);

    private:
        double t_{};
        Shape *object_{};
    };

    class Intersections {
    public:
        Intersections() = default;

        Intersections(const std::initializer_list<Intersection> xs) : intersections_(xs) {
        }

        explicit Intersections(const std::vector<Intersection> &xs) : intersections_(xs) {
        }

        const std::vector<Intersection> &data();

        std::optional<Intersection> hit();

        void insert(const std::vector<Intersection> &xs);

    private:
        void sort();

        std::vector<Intersection> intersections_{};
        bool is_sorted_ = false;
    };

    constexpr double shadow_epsilon = 1e-10;

    struct Computations {
        double t;
        Shape *object;
        Point point;
        Vector eye_v;
        Vector normal_v;
        bool inside;
        Point over_point;
    };

    Computations prepare_computations(const Intersection &intersection, const Ray &ray);
} // rt
