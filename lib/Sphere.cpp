#include "Sphere.h"
#include "Intersection.h"

#include <cmath>

namespace rt {
    std::vector<Intersection> Sphere::intersect(const Ray &ray) {
        const Vector sphere_to_ray = ray.origin() - point(0, 0, 0);

        const auto a = dot(ray.direction(), ray.direction());
        const auto b = 2 * dot(ray.direction(), sphere_to_ray);
        const auto c = dot(sphere_to_ray, sphere_to_ray) - 1;

        const auto discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return {};
        }

        const auto t1 = (-b - std::sqrt(discriminant)) / (2 * a);
        const auto t2 = (-b + std::sqrt(discriminant)) / (2 * a);
        return {{t1, this}, {t2, this}};
    }
}
