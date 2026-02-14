#include "Tuple.h"
#include <cmath>

namespace rt {

    double Tuple::magnitude() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Tuple Tuple::normalize() const {
        const auto m = magnitude();
        return Tuple{x / m, y / m, z / m, w / m};
    }
}
