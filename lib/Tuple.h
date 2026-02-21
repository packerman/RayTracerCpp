#pragma once
#include <ostream>
#include <tuple>
#include <cmath>

namespace rt {
    struct Tuple;
    using Point = Tuple;
    using Vector = Tuple;
    using Color = Tuple;

    constexpr double dot(const Tuple &a, const Tuple &b);

    struct Tuple {
        double x;
        double y;
        double z;
        double w;

        Tuple() = delete;

        constexpr Tuple(const double x, const double y, const double z, const double w) : x(x), y(y), z(z), w(w) {
        }

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
            if (m == 0.0) {
                return *this;
            }
            return Tuple{x / m, y / m, z / m, w / m};
        }

        [[nodiscard]] Tuple reflect(const Tuple &normal) const {
            return *this - normal * 2 * dot(*this, normal);
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

    constexpr Point point(const double x, const double y, const double z) {
        return {x, y, z, 1.0};
    }

    constexpr Vector vector(const double x, const double y, const double z) {
        return {x, y, z, 0.0};
    }

    constexpr Color color(const double red, const double green, const double blue) {
        return Tuple{red, green, blue, 0.0};
    }

    constexpr Color black{color(0, 0, 0)};

    constexpr double dot(const Tuple &a, const Tuple &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    constexpr Tuple cross(const Tuple &a, const Tuple &b) {
        return vector(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x);
    }
}
