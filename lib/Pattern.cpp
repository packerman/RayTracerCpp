#include "Pattern.h"

#include "Shape.h"

namespace rt {
    Color Pattern::at_shape(const Shape &shape, const Point &world_point) const {
        const auto object_point = shape.world_to_object(world_point);
        const auto pattern_point = inversed_transform() * object_point;

        return at(pattern_point);
    }

    std::unique_ptr<SolidPattern> solid_pattern(const Color &c) {
        return std::make_unique<SolidPattern>(c);
    }

    std::unique_ptr<StripePattern> stripe_pattern(const Color &a, const Color &b) {
        return std::make_unique<StripePattern>(a, b);
    }

    std::unique_ptr<GradientPattern> gradient_pattern(const Color &c_a, const Color &c_b) {
        return std::make_unique<GradientPattern>(c_a, c_b);
    }

    std::unique_ptr<RingPattern> ring_pattern(const Color &c_a, const Color &c_b) {
        return std::make_unique<RingPattern>(c_a, c_b);
    }

    std::unique_ptr<CheckersPattern> checkers_pattern(const Color &c_a, const Color &c_b) {
        return std::make_unique<CheckersPattern>(c_a, c_b);
    }
}
