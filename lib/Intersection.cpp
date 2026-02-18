#include "Intersection.h"

#include <algorithm>

namespace rt {

    std::optional<Intersection> hit(std::vector<Intersection> &xs) {
        std::ranges::sort(xs, [](auto& a, auto& b) { return a.t() < b.t();});
        const auto it = std::ranges::find_if(xs, [](auto &x) { return x.t() >= 0; });
        if (it == xs.end()) {
            return {};
        }
        return *it;
    }

    std::ostream & operator<<(std::ostream &os, const Intersection &obj) {
        return os
               << "t_: " << obj.t_
               << " object_: " << obj.object_;
    }
} // rt
