#include "tuple.h"

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

    Tuple Tuple::point(double x, double y, double z) {
        return {x, y, z, 1.0};
    }

    Tuple Tuple::vector(double x, double y, double z) {
        return {x, y, z, 0.0};
    }
}