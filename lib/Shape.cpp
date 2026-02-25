#include "Shape.h"
#include "Intersection.h"

#include <cmath>

namespace rt {
    std::vector<Intersection> Shape::intersect(const Ray &ray) {
        const auto local_ray = ray.transform(inversed_transform());
        return local_intersect(local_ray);
    }

    Vector Shape::normal_at(const Point &point) {
        const auto local_point = inversed_transform() * point;
        const auto local_normal = local_normal_at(local_point);
        auto world_normal = inversed_transform().transpose() * local_normal;
        world_normal.w = 0;
        return world_normal.normalize();
    }

    std::vector<Intersection> Sphere::local_intersect(const Ray &ray) {
        const Vector sphere_to_ray = ray.origin() - point(0, 0, 0);

        const auto a = ray.direction().dot(ray.direction());
        const auto b = 2 * ray.direction().dot(sphere_to_ray);
        const auto c = sphere_to_ray.dot(sphere_to_ray) - 1;

        const auto discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return {};
        }

        const auto t1 = (-b - std::sqrt(discriminant)) / (2 * a);
        const auto t2 = (-b + std::sqrt(discriminant)) / (2 * a);
        return {{t1, this}, {t2, this}};
    }

    Vector Sphere::local_normal_at(const Point &local_point) {
        return local_point - point(0, 0, 0);
    }

    std::unique_ptr<Sphere> sphere() {
        return std::make_unique<Sphere>();
    }

    std::vector<Intersection> Plane::local_intersect(const Ray &ray) {
        if (std::abs(ray.direction().y) < std::numeric_limits<double>::epsilon()) {
            return {};
        }
        auto t = -ray.origin().y / ray.direction().y;
        return {{t, this}};
    }

    Vector Plane::local_normal_at(const Point &point) {
        return vector(0, 1, 0);
    }

    std::unique_ptr<Plane> plane() {
        return std::make_unique<Plane>();
    }
}
