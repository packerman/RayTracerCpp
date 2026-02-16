#pragma once

#include <vector>

#include "Ray.h"

namespace rt {
    class Sphere {
    public:
        std::vector<double> intersect(const Ray& ray);
    };
}
