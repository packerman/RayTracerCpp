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

    constexpr Transformation shearing(const double x_y, const double x_z, const double y_x, const double y_z,
                                      const double z_x, const double z_y) {
        const Transformation t{
            1, x_y, x_z, 0,
            y_x, 1, y_z, 0,
            z_x, z_y, 1, 0,
            0, 0, 0, 1
        };
        return t;
    }

    inline Transformation view_transform(const Point& from, const Point& to, const Vector& up) {
        const auto forward = (to - from).normalize();
        const auto up_n = up.normalize();
        const auto left = forward.cross(up_n);
        const auto true_up = left.cross(forward);

        const Transformation orientation{
            left.x, left.y, left.z, 0,
            true_up.x, true_up.y, true_up.z, 0,
            -forward.x, -forward.y, -forward.z, 0,
            0, 0, 0, 1
        };
        return orientation * translation(-from.x, -from.y, -from.z);
    }
}
