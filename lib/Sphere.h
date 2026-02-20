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

        void set_transform(const Transformation &transform) {
            transform_ = transform;
        }

        std::vector<Intersection> intersect(const Ray &ray);

    private:
        Transformation transform_{Transformation::identity()};
    };
}
