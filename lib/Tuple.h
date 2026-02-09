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

        Tuple operator/(double s) const;

        [[nodiscard]] double magnitude() const;

        static Tuple point(double x, double y, double z);

        static Tuple vector(double x, double y, double z);

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
    };

    using Point = Tuple;
    using Vector = Tuple;
}
