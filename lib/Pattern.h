#pragma once
#include <memory>

#include "Transformation.h"
#include "Tuple.h"

namespace rt {
    class Shape;

    class Pattern {
    public:
        virtual ~Pattern() = default;

        [[nodiscard]] virtual Color at(const Point &point) const = 0;

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

    class SolidPattern : public Pattern {
    public:
        explicit SolidPattern(const Color &c): c(c) {
        }

        Color at(const Point &point) const override {
            return c;
        }

    private:
        Color c;
    };

    std::unique_ptr<SolidPattern> solid_pattern(const Color &c);

    struct StripePattern : Pattern {
        StripePattern(const Color &a, const Color &b)
            : a(a), b(b) {
        }

        [[nodiscard]] Color at(const Point &point) const override {
            return std::fmod(std::floor(point.x), 2) == 0 ? a : b;
        }

        Color a;
        Color b;
    };

    std::unique_ptr<StripePattern> stripe_pattern(const Color &a, const Color &b);

    class GradientPattern : public Pattern {
    public:
        GradientPattern(const Color &c_a, const Color &c_b)
            : c_a(c_a), c_b(c_b) {
        }

        [[nodiscard]] Color at(const Point &point) const override {
            const auto distance = c_b - c_a;
            const auto fraction = point.x - std::floor(point.x);
            return c_a + distance * fraction;
        }

    private:
        Color c_a;
        Color c_b;
    };

    std::unique_ptr<GradientPattern> gradient_pattern(const Color &c_a, const Color &c_b);

    class RingPattern : public Pattern {
    public:
        RingPattern(const Color &c_a, const Color &c_b)
            : c_a(c_a), c_b(c_b) {
        }

        [[nodiscard]] Color at(const Point &point) const override {
            return std::fmod(std::floor(std::hypot(point.x, point.z)), 2) == 0 ? c_a : c_b;
        }

    private:
        Color c_a;
        Color c_b;
    };

    std::unique_ptr<RingPattern> ring_pattern(const Color &c_a, const Color &c_b);

    class CheckersPattern : public Pattern {
    public:
        CheckersPattern(const Color &c_a, const Color &c_b) : c_a(c_a), c_b(c_b) {
        }

        [[nodiscard]] Color at(const Point &point) const override {
            return std::fmod(std::floor(point.x) + std::floor(point.y) + std::floor(point.z), 2) == 0 ? c_a : c_b;
        }

    private:
        Color c_a;
        Color c_b;
    };

    std::unique_ptr<CheckersPattern> checkers_pattern(const Color &c_a, const Color &c_b);
}
