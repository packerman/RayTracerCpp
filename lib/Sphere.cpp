#include "Sphere.h"

#include <cmath>

namespace rt {
    std::vector<double> Sphere::intersect(const Ray &ray) {
        Vector sphere_to_ray = ray.origin() - point(0, 0, 0);

        auto a = dot(ray.direction(), ray.direction());
        auto b = 2 * dot(ray.direction(), sphere_to_ray);
        auto c = dot(sphere_to_ray, sphere_to_ray) - 1;

        auto discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return {};
        }

        auto t1 = (-b - std::sqrt(discriminant)) / (2 * a);
        auto t2 = (-b + std::sqrt(discriminant)) / (2 * a);
        return {t1, t2};
    }
}
