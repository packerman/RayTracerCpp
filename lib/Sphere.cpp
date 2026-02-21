#include "Sphere.h"
#include "Intersection.h"

#include <cmath>

namespace rt {
    std::vector<Intersection> Sphere::intersect(const Ray &ray) {
        const auto ray2 = ray.transform(inversed_transform());

        const Vector sphere_to_ray = ray2.origin() - point(0, 0, 0);

        const auto a = ray2.direction().dot( ray2.direction());
        const auto b = 2 * ray2.direction().dot(sphere_to_ray);
        const auto c = sphere_to_ray.dot(sphere_to_ray) - 1;

        const auto discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return {};
        }

        const auto t1 = (-b - std::sqrt(discriminant)) / (2 * a);
        const auto t2 = (-b + std::sqrt(discriminant)) / (2 * a);
        return {{t1, this}, {t2, this}};
    }

    Vector Sphere::normal_at(const Point &world_point) {
        const auto object_point = inversed_transform() * world_point;
        const auto object_normal = object_point - point(0, 0, 0);
        auto world_normal = inversed_transform().transpose() * object_normal;
        world_normal.w = 0;
        return world_normal.normalize();
    }
}
