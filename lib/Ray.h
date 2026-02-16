#pragma once

#include "Tuple.h"

namespace rt {
    class Ray {
    public:
        constexpr Ray(const Point &origin, const Vector &direction)
            : origin_(origin),
              direction_(direction) {
        }

        [[nodiscard]] constexpr const Point &origin() const {
            return origin_;
        }

        [[nodiscard]] constexpr const Vector &direction() const {
            return direction_;
        }

        [[nodiscard]] constexpr Point position(const double t) const {
            return origin_ + direction_ * t;
        }

    private:
        Point origin_;
        Vector direction_;
    };
}
