#pragma once
#include <memory>

#include "Transformation.h"
#include "Tuple.h"

namespace rt {
    class Shape;

    class Pattern {
    public:
        virtual ~Pattern() = default;

        [[nodiscard]] virtual Color at(const Point &p) const = 0;

        [[nodiscard]] Color at_shape(const Shape &shape, const Point &world_point) const;

        [[nodiscard]] const Transformation &transform() const {
            return transform_;
        }

        [[nodiscard]] const Transformation &inversed_transform() const {
            return inversed_transform_;
        }

        void set_transform(const Transformation &transform) {
            transform_ = transform;
            if (const auto inversed = transform.inverse(); inversed) {
                inversed_transform_ = inversed.value();
            } else {
                throw std::invalid_argument("Matrix is not invertible.");
            }
        }

    private:
        Transformation transform_{Transformation::identity()};
        Transformation inversed_transform_{Transformation::identity()};
    };

    struct StripePattern : Pattern {
        constexpr StripePattern(const Color &a, const Color &b)
            : a(a), b(b) {
        }

        [[nodiscard]] Color at(const Point &p) const override {
            return std::fmod(std::floor(p.x), 2) == 0 ? a : b;
        }

        Color a;
        Color b;

    private:
        Transformation transform_{Transformation::identity()};
        Transformation inversed_transform_{Transformation::identity()};
    };

    std::unique_ptr<StripePattern> stripe_pattern(const Color &a, const Color &b);
}
