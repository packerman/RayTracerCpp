#pragma once
#include <tuple>

namespace rt {
    struct Tuple {
        double x;
        double y;
        double z;
        double w;

        [[nodiscard]] bool is_point() const;

        [[nodiscard]] bool is_vector() const;

        Tuple operator+(const Tuple & tuple) const;

        Tuple operator-(const Tuple & tuple) const;

        static Tuple point(double x, double y, double z);

        static Tuple vector(double x, double y, double z);

        friend bool operator==(const Tuple &lhs, const Tuple &rhs) {
            return std::tie(lhs.x, lhs.y, lhs.z, lhs.w) == std::tie(rhs.x, rhs.y, rhs.z, rhs.w);
        }

        friend bool operator!=(const Tuple &lhs, const Tuple &rhs) {
            return !(lhs == rhs);
        }
    };
}
