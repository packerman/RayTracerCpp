#pragma once
#include <ostream>
#include <tuple>

namespace rt {
    struct Tuple {
        double x;
        double y;
        double z;
        double w;

        [[nodiscard]] bool is_point() const;

        [[nodiscard]] bool is_vector() const;

        Tuple operator+(const Tuple &tuple) const;

        Tuple operator-(const Tuple &tuple) const;

        Tuple operator-() const;

        Tuple operator*(double s) const;

        Tuple operator*(const Tuple& tuple) const;

        Tuple operator/(double s) const;

        [[nodiscard]] double magnitude() const;

        [[nodiscard]] Tuple normalize() const;

        [[nodiscard]] double dot(const Tuple& other) const;

        [[nodiscard]] Tuple cross(const Tuple& other) const;

        friend bool operator==(const Tuple &lhs, const Tuple &rhs) {
            return std::tie(lhs.x, lhs.y, lhs.z, lhs.w) == std::tie(rhs.x, rhs.y, rhs.z, rhs.w);
        }

        friend bool operator!=(const Tuple &lhs, const Tuple &rhs) {
            return !(lhs == rhs);
        }

        friend std::ostream &operator<<(std::ostream &os, const Tuple &obj) {
            return os
                   << "(" << obj.x
                   << ", " << obj.y
                   << ", " << obj.z
                   << ", " << obj.w
                   << ")";
        }

        [[nodiscard]] double red() const {
            return x;
        }

        [[nodiscard]] double green() const {
            return y;
        }

        [[nodiscard]] double blue() const {
            return z;
        }
    };

    using Point = Tuple;

    Point point(double x, double y, double z);

    using Vector = Tuple;

    Vector vector(double x, double y, double z);

    using Color = Tuple;

    Color color(double red, double green, double blue);
}
