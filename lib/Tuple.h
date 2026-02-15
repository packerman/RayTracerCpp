#pragma once
#include <ostream>
#include <tuple>
#include <cmath>

namespace rt {
    struct Tuple {
        double x;
        double y;
        double z;
        double w;

        [[nodiscard]] constexpr bool is_point() const {
            return w == 1.0;
        }

        [[nodiscard]] constexpr bool is_vector() const {
            return w == 0.0;
        }

        constexpr Tuple operator+(const Tuple &tuple) const {
            return Tuple{x + tuple.x, y + tuple.y, z + tuple.z, w + tuple.w};
        }

        constexpr Tuple operator-(const Tuple &tuple) const {
            return Tuple{x - tuple.x, y - tuple.y, z - tuple.z, w - tuple.w};
        }

        constexpr Tuple operator-() const {
            return Tuple{-x, -y, -z, -w};
        }

        constexpr Tuple operator*(const double s) const {
            return Tuple{x * s, y * s, z * s, w * s};
        }

        constexpr Tuple operator*(const Tuple &tuple) const {
            return Tuple{x * tuple.x, y * tuple.y, z * tuple.z, w * tuple.w};
        }

        constexpr Tuple operator/(const double s) const {
            return Tuple{x / s, y / s, z / s, w / s};
        }

        [[nodiscard]] double magnitude() const {
            return std::sqrt(x * x + y * y + z * z + w * w);
        }

        [[nodiscard]] Tuple normalize() const {
            const auto m = magnitude();
            return Tuple{x / m, y / m, z / m, w / m};
        }

        friend constexpr bool operator==(const Tuple &lhs, const Tuple &rhs) {
            return std::tie(lhs.x, lhs.y, lhs.z, lhs.w) == std::tie(rhs.x, rhs.y, rhs.z, rhs.w);
        }

        friend constexpr bool operator!=(const Tuple &lhs, const Tuple &rhs) {
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

        [[nodiscard]] constexpr double red() const {
            return x;
        }

        [[nodiscard]] constexpr double green() const {
            return y;
        }

        [[nodiscard]] constexpr double blue() const {
            return z;
        }
    };

    using Point = Tuple;

    constexpr Point point(const double x, const double y, const double z) {
        return {x, y, z, 1.0};
    }

    using Vector = Tuple;

    constexpr Vector vector(const double x, const double y, const double z) {
        return {x, y, z, 0.0};
    }

    using Color = Tuple;

    constexpr Color color(const double red, const double green, const double blue) {
        return Tuple{red, green, blue, 0.0};
    }

    constexpr double dot(const Tuple& a, const Tuple &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    constexpr Tuple cross(const Tuple& a, const Tuple &b) {
        return vector(
                    a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x);
    }
}
