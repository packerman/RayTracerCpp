#pragma once
#include <memory>

#include "Shape.h"

namespace rt {
    struct TestShape : Shape {
        std::vector<Intersection> local_intersect(const Ray& ray) override {
            saved_ray = ray;
            return {};
        }

        [[nodiscard]] Vector local_normal_at(const Point& point) const override {
            return vector(point.x, point.y, point.z);
        }

        [[nodiscard]] Bounds bounds() const override {
            return {
                {
                    point(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                          -std::numeric_limits<double>::infinity())
                },
                {
                    point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
                          std::numeric_limits<double>::infinity())
                }
            };
        }

        Ray saved_ray{point(0, 0, 0), vector(0, 0, 0)};
    };

    std::unique_ptr<TestShape> test_shape();

    std::unique_ptr<Shape> glass_sphere();
}
