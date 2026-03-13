#include "Shape.h"
#include "Intersection.h"

#include <cmath>
#include <algorithm>

namespace rt {
    std::vector<Intersection> Shape::intersect(const Ray &ray) {
        const auto local_ray = ray.transform(inversed_transform());
        return local_intersect(local_ray);
    }

    Vector Shape::normal_at(const Point &point) const {
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

    Vector Sphere::local_normal_at(const Point &local_point) const {
        return local_point - point(0, 0, 0);
    }

    std::unique_ptr<Shape> sphere() {
        return std::make_unique<Sphere>();
    }

    std::vector<Intersection> Plane::local_intersect(const Ray &ray) {
        if (std::abs(ray.direction().y) < std::numeric_limits<double>::epsilon()) {
            return {};
        }
        auto t = -ray.origin().y / ray.direction().y;
        return {{t, this}};
    }

    Vector Plane::local_normal_at(const Point &point) const {
        return vector(0, 1, 0);
    }

    std::unique_ptr<Shape> plane() {
        return std::make_unique<Plane>();
    }

    std::vector<Intersection> Cube::local_intersect(const Ray &ray) {
        auto [xt_min, xt_max] = check_axis(ray.origin().x, ray.direction().x);
        auto [yt_min, yt_max] = check_axis(ray.origin().y, ray.direction().y);
        auto [zt_min, zt_max] = check_axis(ray.origin().z, ray.direction().z);

        auto t_min = std::max({xt_min, yt_min, zt_min});
        auto t_max = std::min({xt_max, yt_max, zt_max});

        if (t_min > t_max) {
            return {};
        }

        return {{t_min, this}, {t_max, this}};
    }

    Vector Cube::local_normal_at(const Point &local_point) const {
        const auto max_c = std::max({std::abs(local_point.x), std::abs(local_point.y), std::abs(local_point.z)});

        if (max_c == std::abs(local_point.x)) {
            return vector(local_point.x, 0, 0);
        }
        if (max_c == std::abs(local_point.y)) {
            return vector(0, local_point.y, 0);
        }
        return vector(0, 0, local_point.z);
    }

    std::pair<double, double> Cube::check_axis(const double origin, const double direction) {
        const auto t_min_numerator = -1 - origin;
        const auto t_max_numerator = 1 - origin;

        auto t_min = t_min_numerator / direction;
        auto t_max = t_max_numerator / direction;

        if (t_min > t_max) {
            std::swap(t_min, t_max);
        }

        return {t_min, t_max};
    }

    std::unique_ptr<Shape> cube() {
        return std::make_unique<Cube>();
    }

    std::vector<Intersection> Cylinder::local_intersect(const Ray &ray) {
        const auto a = std::pow(ray.direction().x, 2) + std::pow(ray.direction().z, 2);
        if (a < std::numeric_limits<double>::epsilon()) {
            return {};
        }
        const auto b = 2 * ray.origin().x * ray.direction().x +
                       2 * ray.origin().z * ray.direction().z;
        const auto c = std::pow(ray.origin().x, 2) + std::pow(ray.origin().z, 2) - 1;
        const auto disc = std::pow(b, 2) - 4 * a * c;
        if (disc < 0) {
            return {};
        }
        auto t0 = (-b - std::sqrt(disc)) / (2 * a);
        auto t1 = (-b + std::sqrt(disc)) / (2 * a);
        if (t0 > t1) {
            std::swap(t0, t1);
        }

        std::vector<Intersection> xs;

        if (const auto y0 = ray.origin().y + t0 * ray.direction().y; minimum_ < y0 && y0 < maximum_) {
            xs.emplace_back(t0, this);
        }

        if (const auto y1 = ray.origin().y + t1 * ray.direction().y; minimum_ < y1 && y1 < maximum_) {
            xs.emplace_back(t1, this);
        }

        return xs;
    }

    Vector Cylinder::local_normal_at(const Point &local_point) const {
        return vector(local_point.x, 0, local_point.z);
    }

    std::unique_ptr<Shape> cylinder(const double minimum, const double maximum) {
        return std::make_unique<Cylinder>(minimum, maximum);
    }
}
