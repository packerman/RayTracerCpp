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

    Tuple Tuple::operator/(const double s) const {
        return Tuple{x / s, y / s, z / s, w / s};
    }

    double Tuple::magnitude() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Tuple Tuple::point(const double x, const double y, const double z) {
        return {x, y, z, 1.0};
    }

    Tuple Tuple::vector(const double x, const double y, const double z) {
        return {x, y, z, 0.0};
    }
}
