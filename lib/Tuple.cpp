#include "Tuple.h"
#include <cmath>

namespace rt {
    bool Tuple::is_point() const {
        return w == 1.0;
    }

    bool Tuple::is_vector() const {
        return w == 0.0;
    }

    Tuple Tuple::operator+(const Tuple &tuple) const {
        return Tuple{x + tuple.x, y + tuple.y, z + tuple.z, w + tuple.w};
    }

    Tuple Tuple::operator-(const Tuple &tuple) const {
        return Tuple{x - tuple.x, y - tuple.y, z - tuple.z, w - tuple.w};
    }

    Tuple Tuple::operator-() const {
        return Tuple{-x, -y, -z, -w};
    }

    Tuple Tuple::operator*(const double s) const {
        return Tuple{x * s, y * s, z * s, w * s};
    }

    Tuple Tuple::operator*(const Tuple &tuple) const {
        return Tuple{x * tuple.x, y * tuple.y, z * tuple.z, w * tuple.w};
    }

    Tuple Tuple::operator/(const double s) const {
        return Tuple{x / s, y / s, z / s, w / s};
    }

    double Tuple::magnitude() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Tuple Tuple::normalize() const {
        const auto m = magnitude();
        return Tuple{x / m, y / m, z / m, w / m};
    }

    double Tuple::dot(const Tuple &other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    Tuple Tuple::cross(const Tuple &other) const {
        return vector(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x);
    }

    Point point(const double x, const double y, const double z) {
        return {x, y, z, 1.0};
    }

    Vector vector(const double x, const double y, const double z) {
        return {x, y, z, 0.0};
    }

    Color color(const double red, const double green, const double blue) {
        return Tuple{red, green, blue, 0.0};
    }
}
