#include "Shape.h"
#include "Intersection.h"

#include <cmath>
#include <algorithm>

using namespace std;

namespace rt {
    std::vector<Intersection> Shape::intersect(const Ray& ray) {
        const auto local_ray = ray.transform(inversed_transform());
        return local_intersect(local_ray);
    }

    Vector Shape::normal_at(const Point& point) const {
        const auto local_point = inversed_transform() * point;
        const auto local_normal = local_normal_at(local_point);
        auto world_normal = inversed_transform().transpose() * local_normal;
        world_normal.w = 0;
        return world_normal.normalize();
    }

    std::vector<Intersection> Sphere::local_intersect(const Ray& ray) {
        const Vector sphere_to_ray = ray.origin() - point(0, 0, 0);

        const auto a = ray.direction().dot(ray.direction());
        const auto b = 2 * ray.direction().dot(sphere_to_ray);
        const auto c = sphere_to_ray.dot(sphere_to_ray) - 1;

        const auto discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return {};
        }

        const auto t1 = (-b - sqrt(discriminant)) / (2 * a);
        const auto t2 = (-b + sqrt(discriminant)) / (2 * a);
        return {{t1, this}, {t2, this}};
    }

    Vector Sphere::local_normal_at(const Point& local_point) const {
        return local_point - point(0, 0, 0);
    }

    unique_ptr<Shape> sphere() {
        return make_unique<Sphere>();
    }

    std::vector<Intersection> Plane::local_intersect(const Ray& ray) {
        if (std::abs(ray.direction().y) < numeric_limits<double>::epsilon()) {
            return {};
        }
        auto t = -ray.origin().y / ray.direction().y;
        return {{t, this}};
    }

    Vector Plane::local_normal_at(const Point& point) const {
        return vector(0, 1, 0);
    }

    unique_ptr<Shape> plane() {
        return make_unique<Plane>();
    }

    std::vector<Intersection> Cube::local_intersect(const Ray& ray) {
        auto [xt_min, xt_max] = check_axis(ray.origin().x, ray.direction().x);
        auto [yt_min, yt_max] = check_axis(ray.origin().y, ray.direction().y);
        auto [zt_min, zt_max] = check_axis(ray.origin().z, ray.direction().z);

        auto t_min = max({xt_min, yt_min, zt_min});
        auto t_max = min({xt_max, yt_max, zt_max});

        if (t_min > t_max) {
            return {};
        }

        return {{t_min, this}, {t_max, this}};
    }

    Vector Cube::local_normal_at(const Point& local_point) const {
        const auto max_c = max({abs(local_point.x), abs(local_point.y), abs(local_point.z)});

        if (max_c == abs(local_point.x)) {
            return vector(local_point.x, 0, 0);
        }
        if (max_c == abs(local_point.y)) {
            return vector(0, local_point.y, 0);
        }
        return vector(0, 0, local_point.z);
    }

    pair<double, double> Cube::check_axis(const double origin, const double direction) {
        const auto t_min_numerator = -1 - origin;
        const auto t_max_numerator = 1 - origin;

        auto t_min = t_min_numerator / direction;
        auto t_max = t_max_numerator / direction;

        if (t_min > t_max) {
            swap(t_min, t_max);
        }

        return {t_min, t_max};
    }

    unique_ptr<Shape> cube() {
        return make_unique<Cube>();
    }

    std::vector<Intersection> Cylinder::local_intersect(const Ray& ray) {
        std::vector<Intersection> xs;
        if (const auto a = pow(ray.direction().x, 2) + pow(ray.direction().z, 2);
            a > numeric_limits<double>::epsilon()) {
            const auto b = 2 * ray.origin().x * ray.direction().x + 2 * ray.origin().z * ray.direction().z;
            const auto c = pow(ray.origin().x, 2) + pow(ray.origin().z, 2) - 1;
            const auto disc = b * b - 4 * a * c;
            if (disc < 0) {
                return {};
            }
            auto t0 = (-b - sqrt(disc)) / (2 * a);
            auto t1 = (-b + sqrt(disc)) / (2 * a);

            if (const auto y0 = ray.origin().y + t0 * ray.direction().y; minimum_ < y0 && y0 < maximum_) {
                xs.emplace_back(t0, this);
            }

            if (const auto y1 = ray.origin().y + t1 * ray.direction().y; minimum_ < y1 && y1 < maximum_) {
                xs.emplace_back(t1, this);
            }
        }

        intersect_caps(ray, xs);

        return xs;
    }

    Vector Cylinder::local_normal_at(const Point& local_point) const {
        const auto dist = pow(local_point.x, 2) + pow(local_point.z, 2);

        if (dist < 1 && local_point.y >= maximum_ - numeric_limits<double>::epsilon()) {
            return vector(0, 1, 0);
        }
        if (dist < 1 && local_point.y <= minimum_ + numeric_limits<double>::epsilon()) {
            return vector(0, -1, 0);
        }
        return vector(local_point.x, 0, local_point.z);
    }

    void Cylinder::intersect_caps(const Ray& ray, std::vector<Intersection>& xs) {
        if (!closed_ || abs(ray.direction().y) <= numeric_limits<double>::epsilon()) {
            return;
        }
        auto t = (minimum_ - ray.origin().y) / ray.direction().y;
        if (check_cap(ray, t)) {
            xs.emplace_back(t, this);
        }

        t = (maximum_ - ray.origin().y) / ray.direction().y;
        if (check_cap(ray, t)) {
            xs.emplace_back(t, this);
        }
    }

    bool Cylinder::check_cap(const Ray& ray, double t) {
        const auto x = ray.origin().x + t * ray.direction().x;
        const auto z = ray.origin().z + t * ray.direction().z;

        return x * x + z * z <= 1;
    }

    unique_ptr<Shape> cylinder(double minimum, double maximum, bool closed) {
        return make_unique<Cylinder>(minimum, maximum, closed);
    }
}
