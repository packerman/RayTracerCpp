#pragma once

#include <vector>

#include "Ray.h"
#include "Intersection.h"
#include "Transformation.h"

namespace rt {
    class Sphere {
    public:
        Sphere() = default;

        Sphere(const Sphere &other) = delete;

        Sphere(Sphere &&other) noexcept = delete;

        Sphere &operator=(const Sphere &other) = delete;

        Sphere &operator=(Sphere &&other) noexcept = delete;

        [[nodiscard]] const Transformation &transform() const {
            return transform_;
        }

        [[nodiscard]] const Transformation &inversed_transform() const {
            return inversed_transform_;
        }

        void set_transform(const Transformation &transform) {
            transform_ = transform;
            inversed_transform_ = transform.inverse();
        }

        std::vector<Intersection> intersect(const Ray &ray);

        Vector normal_at(const Point& world_point);

    private:
        Transformation transform_{Transformation::identity()};
        Transformation inversed_transform_{Transformation::identity()};
    };
}
