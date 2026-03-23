#pragma once

#include <ostream>
#include <vector>

#include "Transformation.h"
#include "Tuple.h"

namespace rt {
    struct Bounds {
        Point minimum;
        Point maximum;

        [[nodiscard]] Bounds transform(const Transformation& t) const;

        friend bool operator==(const Bounds& lhs, const Bounds& rhs);

        friend bool operator!=(const Bounds& lhs, const Bounds& rhs);

        friend std::ostream& operator<<(std::ostream& os, const Bounds& obj);
    };

    Bounds combine_bounds(const std::vector<Bounds>& boxes);
}
