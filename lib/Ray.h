#pragma once

#include <ostream>

#include "Tuple.h"
#include "Transformation.h"

namespace rt {
    class Ray {
    public:
        constexpr Ray(const Point& origin, const Vector& direction)
            : origin_(origin),
              direction_(direction) {
        }

        [[nodiscard]] constexpr const Point& origin() const {
            return origin_;
        }

        [[nodiscard]] constexpr const Vector& direction() const {
            return direction_;
        }

        friend bool operator==(const Ray& lhs, const Ray& rhs) {
            return std::tie(lhs.origin_, lhs.direction_) == std::tie(rhs.origin_, rhs.direction_);
        }

        friend bool operator!=(const Ray& lhs, const Ray& rhs) {
            return !(lhs == rhs);
        }

        [[nodiscard]] constexpr Point position(const double t) const {
            return origin_ + direction_ * t;
        }

        [[nodiscard]] constexpr Ray transform(const Transformation& m) const {
            return {m * origin_, m * direction_};
        }

        friend std::ostream& operator<<(std::ostream& os, const Ray& obj);

    private:
        Point origin_;
        Vector direction_;
    };

    Ray ray(const Point& origin, const Vector& direction);
}
