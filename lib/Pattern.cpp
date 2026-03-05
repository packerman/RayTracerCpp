#include "Pattern.h"

namespace rt {
    std::unique_ptr<StripePattern> stripe_pattern(const Color &a, const Color &b) {
        return std::make_unique<StripePattern>(a, b);
    }
}
