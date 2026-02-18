#pragma once

#include <vector>

#include "Ray.h"
#include "Intersection.h"

namespace rt {

    class Sphere {
    public:
        Sphere() = default;

        Sphere(const Sphere &other) = delete;
        Sphere(Sphere &&other) noexcept = delete;
        Sphere & operator=(const Sphere &other) = delete;
        Sphere & operator=(Sphere &&other) noexcept = delete;

        std::vector<Intersection> intersect(const Ray& ray);
    };
}
