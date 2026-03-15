#include "Group.h"

using namespace std;

namespace rt {
    std::vector<Intersection> Group::local_intersect(const Ray& ray) {
        return {};
    }

    Vector Group::local_normal_at(const Point& local_point) const {
        return {};
    }

    std::unique_ptr<Group> group() {
        return make_unique<Group>();
    }
}
