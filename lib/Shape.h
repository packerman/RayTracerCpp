#pragma once

#include <memory>
#include <vector>

#include "Ray.h"
#include "Intersection.h"
#include "Transformation.h"
#include "Material.h"

namespace rt {
    class Shape {
    public:
        Shape() = default;

        virtual ~Shape() = default;

        Shape(const Shape &other) = delete;

        Shape(Shape &&other) noexcept = delete;

        Shape &operator=(const Shape &other) = delete;

        Shape &operator=(Shape &&other) noexcept = delete;

        [[nodiscard]] const Transformation &transform() const {
            return transform_;
        }

        [[nodiscard]] const Transformation &inversed_transform() const {
            return inversed_transform_;
        }

        void set_transform(const Transformation &transform) {
            transform_ = transform;
            if (const auto inversed = transform.inverse(); inversed) {
                inversed_transform_ = inversed.value();
            } else {
                throw std::invalid_argument("Matrix is not invertible.");
            }
        }

        [[nodiscard]] const Material &material() const {
            return material_;
        }

        [[nodiscard]] Material &material() {
            return material_;
        }

        void set_material(const Material &material) {
            material_ = material;
        }

        std::vector<Intersection> intersect(const Ray &ray);

        virtual std::vector<Intersection> local_intersect(const Ray &ray) = 0;

        Vector normal_at(const Point &point);

        virtual Vector local_normal_at(const Point &local_point) = 0;

    private:
        Transformation transform_ = Transformation::identity();
        Transformation inversed_transform_ = Transformation::identity();
        Material material_{};
    };

    class Sphere : public Shape {
    public:
        Sphere() = default;

        Sphere(const Sphere &other) = delete;

        Sphere(Sphere &&other) noexcept = delete;

        Sphere &operator=(const Sphere &other) = delete;

        Sphere &operator=(Sphere &&other) noexcept = delete;

        std::vector<Intersection> local_intersect(const Ray &ray) override;

        Vector local_normal_at(const Point &local_point) override;
    };

    std::unique_ptr<Sphere> sphere();

    class Plane : public Shape {
    public:
        std::vector<Intersection> local_intersect(const Ray &ray) override;

        Vector local_normal_at(const Point &point) override;
    };

    std::unique_ptr<Plane> plane();
}
