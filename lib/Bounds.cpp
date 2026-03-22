#include "Bounds.h"
#include <algorithm>

using namespace std;

namespace rt {
    Bounds Bounds::transform(const Transformation& t) const {
        auto p0 = t * minimum;
        auto p1 = t * point(maximum.x, minimum.y, minimum.z);
        auto p2 = t * point(minimum.x, minimum.y, maximum.z);
        auto p3 = t * point(maximum.x, minimum.y, maximum.z);
        auto p4 = t * point(minimum.x, maximum.y, minimum.z);
        auto p5 = t * point(maximum.x, maximum.y, minimum.z);
        auto p6 = t * point(minimum.x, maximum.y, maximum.z);
        auto p7 = t * maximum;

        const auto min_x = min({p0.x, p1.x, p2.x, p3.x, p4.x, p5.x, p6.x, p7.x});
        const auto min_y = min({p0.y, p1.y, p2.y, p3.y, p4.y, p5.y, p6.y, p7.y});
        const auto min_z = min({p0.z, p1.z, p2.z, p3.z, p4.z, p5.z, p6.z, p7.z});

        const auto max_x = max({p0.x, p1.x, p2.x, p3.x, p4.x, p5.x, p6.x, p7.x});
        const auto max_y = max({p0.y, p1.y, p2.y, p3.y, p4.y, p5.y, p6.y, p7.y});
        const auto max_z = max({p0.z, p1.z, p2.z, p3.z, p4.z, p5.z, p6.z, p7.z});
        return {
            point(min_x, min_y, min_z),
            point(max_x, max_y, max_z)
        };
    }

    bool operator==(const Bounds& lhs, const Bounds& rhs) {
        return std::tie(lhs.minimum, lhs.maximum) == std::tie(rhs.minimum, rhs.maximum);
    }

    bool operator!=(const Bounds& lhs, const Bounds& rhs) {
        return !(lhs == rhs);
    }

    std::ostream& operator<<(std::ostream& os, const Bounds& obj) {
        return os
               << "minimum: " << obj.minimum
               << " maximum: " << obj.maximum;
    }

    Point min_xyz(const Point& p, const Point& q) {
        return point(
            min(p.x, q.x),
            min(p.y, q.y),
            min(p.z, q.z)
        );
    }
}
