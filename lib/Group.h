#pragma once

#include "Shape.h"

namespace rt {
    class Group : public Shape {
    public:
        std::vector<Intersection> local_intersect(const Ray& ray) override;

        [[nodiscard]] Vector local_normal_at(const Point& local_point) const override;

        [[nodiscard]] Bounds bounds() const override;

        void add_child(std::unique_ptr<Shape> shape);

        bool includes(Shape* shape) const;

        [[nodiscard]] bool empty() const {
            return children_.empty();
        }

    private:
        std::vector<std::unique_ptr<Shape> > children_;
    };

    std::unique_ptr<Group> group();
}
