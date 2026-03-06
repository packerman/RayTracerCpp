#include "Pattern.h"

#include "Shape.h"

namespace rt {
    Color Pattern::at_shape(const Shape &shape, const Point &world_point) const {
        const auto object_point = shape.inversed_transform() * world_point;
        const auto pattern_point = inversed_transform() * object_point;

        return at(pattern_point);
    }

    std::unique_ptr<StripePattern> stripe_pattern(const Color &a, const Color &b) {
        return std::make_unique<StripePattern>(a, b);
    }

    std::unique_ptr<GradientPattern> gradient_pattern(const Color &c_a, const Color &c_b) {
        return std::make_unique<GradientPattern>(c_a, c_b);
    }
}
