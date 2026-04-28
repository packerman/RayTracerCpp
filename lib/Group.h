#pragma once

#include "Shape.h"

#include <optional>

namespace rt {
    class Group : public Shape {
    public:
        std::vector<Intersection> local_intersect(const Ray& ray) override;

        [[nodiscard]] Vector local_normal_at(const Point& local_point) const override;

        [[nodiscard]] Bounds bounds() override;

        void add_child(std::unique_ptr<Shape> shape);

        bool includes(Shape* shape) const;

        [[nodiscard]] bool empty() const {
            return children_.empty();
        }

    private:
        [[nodiscard]] bool intersects_bounds(const Ray& ray);

        std::vector<std::unique_ptr<Shape> > children_;
        std::optional<Bounds> cached_bounds_;
    };

    std::unique_ptr<Group> group();
}
