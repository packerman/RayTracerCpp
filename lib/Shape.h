#pragma once

#include <memory>
#include <vector>

#include "Ray.h"
#include "Intersection.h"
#include "Transformation.h"
#include "Lighting.h"

namespace rt {
    class Shape {
    public:
        Shape() = default;

        virtual ~Shape() = default;

        Shape(const Shape& other) = delete;

        Shape(Shape&& other) noexcept = delete;

        Shape& operator=(const Shape& other) = delete;

        Shape& operator=(Shape&& other) noexcept = delete;

        [[nodiscard]] const Transformation& transform() const {
            return transform_;
        }

        [[nodiscard]] const Transformation& inversed_transform() const {
            return inversed_transform_;
        }

        void set_transform(const Transformation& transform) {
            transform_ = transform;
            if (const auto inversed = transform.inverse(); inversed) {
                inversed_transform_ = inversed.value();
            } else {
                throw std::invalid_argument("Matrix is not invertible.");
            }
        }

        [[nodiscard]] const Material& material() const {
            return material_;
        }

        [[nodiscard]] Material& material() {
            return material_;
        }

        void set_material(const Material& material) {
            material_ = material;
        }

        std::vector<Intersection> intersect(const Ray& ray);

        virtual std::vector<Intersection> local_intersect(const Ray& ray) = 0;

        [[nodiscard]] Vector normal_at(const Point& point) const;

        [[nodiscard]] virtual Vector local_normal_at(const Point& local_point) const = 0;

    private:
        Transformation transform_{Transformation::identity()};
        Transformation inversed_transform_{Transformation::identity()};
        Material material_{};
    };

    class Sphere : public Shape {
    public:
        Sphere() = default;

        Sphere(const Sphere& other) = delete;

        Sphere(Sphere&& other) noexcept = delete;

        Sphere& operator=(const Sphere& other) = delete;

        Sphere& operator=(Sphere&& other) noexcept = delete;

        std::vector<Intersection> local_intersect(const Ray& ray) override;

        [[nodiscard]] Vector local_normal_at(const Point& local_point) const override;
    };

    std::unique_ptr<Shape> sphere();

    class Plane : public Shape {
    public:
        std::vector<Intersection> local_intersect(const Ray& ray) override;

        [[nodiscard]] Vector local_normal_at(const Point& point) const override;
    };

    std::unique_ptr<Shape> plane();

    class Cube : public Shape {
    public:
        std::vector<Intersection> local_intersect(const Ray& ray) override;

        [[nodiscard]] Vector local_normal_at(const Point& local_point) const override;

    private:
        static std::pair<double, double> check_axis(double origin, double direction);
    };

    std::unique_ptr<Shape> cube();

    class Cylinder : public Shape {
    public:
        Cylinder(const double minimum, const double maximum, const bool closed)
            : minimum_(minimum),
              maximum_(maximum),
              closed_(closed) {
        }

        std::vector<Intersection> local_intersect(const Ray& ray) override;

        [[nodiscard]] Vector local_normal_at(const Point& local_point) const override;

        [[nodiscard]] double minimum() const {
            return minimum_;
        }

        [[nodiscard]] double maximum() const {
            return maximum_;
        }

        [[nodiscard]] bool closed() const {
            return closed_;
        }

    private:
        double minimum_;
        double maximum_;
        bool closed_;

        void intersect_caps(const Ray& ray, std::vector<Intersection>& xs);

        static bool check_cap(const Ray& ray, double t);
    };

    std::unique_ptr<Shape> cylinder(double minimum = -std::numeric_limits<double>::infinity(),
                                    double maximum = std::numeric_limits<double>::infinity(),
                                    bool closed = false);

    class Cone : public Shape {
    public:
        Cone(const double minimum, const double maximum, const bool closed)
            : minimum_(minimum),
              maximum_(maximum),
              closed_(closed) {
        }

        std::vector<Intersection> local_intersect(const Ray& ray) override;

        [[nodiscard]] Vector local_normal_at(const Point& local_point) const override;

    private:
        double minimum_;
        double maximum_;
        bool closed_;

        void intersect_caps(const Ray& ray, std::vector<Intersection>& xs);

        static bool check_cap(const Ray& ray, double t, double y);
    };

    std::unique_ptr<Shape> cone(double minimum = -std::numeric_limits<double>::infinity(),
                                double maximum = std::numeric_limits<double>::infinity(),
                                bool closed = false);
}
