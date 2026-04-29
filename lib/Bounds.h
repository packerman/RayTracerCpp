#pragma once

#include <ostream>
#include <vector>

#include "Transformation.h"
#include "Tuple.h"
#include "Ray.h"

namespace rt {
    struct Bounds {
        Point minimum;
        Point maximum;

        [[nodiscard]] Bounds transform(const Transformation& t) const;

        [[nodiscard]] std::vector<double> local_intersect(const Ray& ray) const;

        friend bool operator==(const Bounds& lhs, const Bounds& rhs);

        friend bool operator!=(const Bounds& lhs, const Bounds& rhs);

        friend std::ostream& operator<<(std::ostream& os, const Bounds& obj);

    private:
        static std::pair<double, double> check_axis(double origin, double direction, double axis_min,
                                                    double axis_max);
    };

    Bounds combine_bounds(const std::vector<Bounds>& boxes);
}
