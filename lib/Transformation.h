#pragma once

#include "Matrix.h"

#include <cmath>

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

    inline Transformation rotation_x(const double r) {
        const Transformation t{
            1, 0, 0, 0,
            0, std::cos(r), -std::sin(r), 0,
            0, std::sin(r), std::cos(r), 0,
            0, 0, 0, 1
        };
        return t;
    }

    inline Transformation rotation_y(const double r) {
        const Transformation t{
            std::cos(r), 0, std::sin(r), 0,
            0, 1, 0, 0,
            -std::sin(r), 0, std::cos(r), 0,
            0, 0, 0, 1
        };
        return t;
    }

    inline Transformation rotation_z(const double r) {
        const Transformation t{
            std::cos(r), -std::sin(r), 0, 0,
            std::sin(r), std::cos(r), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
        return t;
    }
}
