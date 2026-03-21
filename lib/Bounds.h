#pragma once

#include <ostream>

#include "Tuple.h"

namespace rt {
    struct Bounds {
        Point minimum;
        Point maximum;

        friend bool operator==(const Bounds& lhs, const Bounds& rhs) {
            return std::tie(lhs.minimum, lhs.maximum) == std::tie(rhs.minimum, rhs.maximum);
        }

        friend bool operator!=(const Bounds& lhs, const Bounds& rhs) {
            return !(lhs == rhs);
        }

        friend std::ostream& operator<<(std::ostream& os, const Bounds& obj) {
            return os
                   << "minimum: " << obj.minimum
                   << " maximum: " << obj.maximum;
        }
    };
}
