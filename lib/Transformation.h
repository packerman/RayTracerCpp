#pragma once

#include "Matrix.h"

namespace rt {
    using Transformation = Matrix<4>;

    constexpr Transformation translation(const double x, const double y, const double z) {
        const Transformation t{
            1, 0, 0, x,
            0, 1, 0, y,
            0, 0, 1, z,
            0, 0, 0, 1
        };
        return t;
    }

    constexpr Transformation scaling(const double x, const double y, const double z) {
        const Transformation t{
            x, 0, 0, 0,
            0, y, 0, 0,
            0, 0, z, 0,
            0, 0, 0, 1
        };
        return t;
    }
}
