#pragma once
#include <ostream>
#include <tuple>
#include <cmath>

namespace rt {
    struct Tuple;
    using Point = Tuple;
    using Vector = Tuple;
    using Color = Tuple;
    constexpr Vector vector(double x, double y, double z);

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

        Tuple &operator+=(const Tuple &tuple) {
            x += tuple.x;
            y += tuple.y;
            z += tuple.z;
            w += tuple.w;
            return *this;
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

        [[nodiscard]] constexpr double dot(const Tuple &other) const {
            return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
        }

        [[nodiscard]] constexpr Tuple cross(const Tuple &other) const {
            return vector(
                this->y * other.z - this->z * other.y,
                this->z * other.x - this->x * other.z,
                this->x * other.y - this->y * other.x);
        }

        [[nodiscard]] Tuple reflect(const Tuple &normal) const {
            return *this - normal * 2 * this->dot(normal);
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
}
