#pragma once
#include <memory>

#include "Tuple.h"

namespace rt {
    struct StripePattern {
        constexpr StripePattern(const Color &a, const Color &b)
            : a(a), b(b) {
        }

        [[nodiscard]] Color color_at(const Point &p) const {
            return std::fmod(std::floor(p.x), 2) == 0 ? a : b;
        }

        Color a;
        Color b;
    };

    std::unique_ptr<StripePattern> stripe_pattern(const Color &a, const Color &b);
}
