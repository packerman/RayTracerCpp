#include "Group.h"

#include <algorithm>

using namespace std;

namespace rt {
    std::vector<Intersection> Group::local_intersect(const Ray& ray) {
        return {};
    }

    Vector Group::local_normal_at(const Point& local_point) const {
        return {};
    }

    void Group::add_child(std::unique_ptr<Shape> shape) {
        shape->parent_ = this;
        children_.push_back(std::move(shape));
    }

    bool Group::includes(Shape* shape) const {
        const auto it = ranges::find_if(children_, [shape](auto& child) {
            return child.get() == shape;
        });
        return it != children_.end();
    }

    std::unique_ptr<Group> group() {
        return make_unique<Group>();
    }
}
