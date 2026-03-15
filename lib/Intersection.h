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

        Intersection(const double t, Shape* object) : t_{t}, object_{object} {
        }

        [[nodiscard]] double t() const {
            return t_;
        }

        [[nodiscard]] Shape* object() const {
            return object_;
        }

        friend bool operator==(const Intersection& lhs, const Intersection& rhs);

        friend bool operator!=(const Intersection& lhs, const Intersection& rhs);

        friend std::ostream& operator<<(std::ostream& os, const Intersection& obj);

        auto operator<=>(const Intersection& other) const;

    private:
        double t_{};
        Shape* object_{};
    };

    class Intersections {
    public:
        Intersections() = default;

        Intersections(const std::initializer_list<Intersection> xs) : intersections_(xs) {
        }

        explicit Intersections(const std::vector<Intersection>& xs) : intersections_(xs) {
        }

        [[nodiscard]] std::size_t size() const {
            return intersections_.size();
        }

        const Intersection& operator[](std::size_t i);

        const std::vector<Intersection>& data();

        std::optional<Intersection> hit();

        void insert(const std::vector<Intersection>& xs);

    private:
        void ensure_sorted();

        std::vector<Intersection> intersections_{};
        bool is_sorted_ = false;
    };

    constexpr double shadow_epsilon = 1e-10;

    struct Computations {
        double t;
        Shape* object;
        Point point;
        Vector eye_v;
        Vector normal_v;
        bool inside;
        Point over_point;
        Point under_point;
        Vector reflect_v;
        double n1;
        double n2;
    };

    Computations prepare_computations(const Intersection& intersection, const Ray& ray, Intersections& xs);

    Computations prepare_computations(const Intersection& intersection, const Ray& ray);

    double schlick(const Computations& comps);
}
