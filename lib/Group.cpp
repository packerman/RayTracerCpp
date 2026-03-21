#include "Group.h"

#include <algorithm>

using namespace std;

namespace rt {
    std::vector<Intersection> Group::local_intersect(const Ray& ray) {
        std::vector<Intersection> result;
        for (const auto& child: children_) {
            auto xs = child->intersect(ray);
            result.insert(result.end(), xs.begin(), xs.end());
        }
        std::sort(result.begin(), result.end());
        return result;
    }

    Vector Group::local_normal_at(const Point& local_point) const {
        throw std::runtime_error("Group::local_normal_at not implemented");
    }

    Bounds Group::bounds() const {
        throw std::runtime_error("Group::bounds not implemented");
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
