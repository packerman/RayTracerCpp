#pragma once
#include <memory>
#include <vector>

#include "Sphere.h"

namespace rt {
    class World {
    public:
        [[nodiscard]] World() = default;

        [[nodiscard]] const std::vector<std::unique_ptr<Sphere> > &objects() const {
            return objects_;
        }

        [[nodiscard]] const std::unique_ptr<Sphere> &object(const std::size_t index) const {
            return objects_[index];
        }

        [[nodiscard]] const std::optional<Light> &light() const {
            return light_;
        }

        void add_object(std::unique_ptr<Sphere> object) {
            objects_.emplace_back(std::move(object));
        }

        void set_light(const Light &light) {
            light_ = light;
        }

        [[nodiscard]] std::vector<Intersection> intersect(const Ray &ray) const;

    private:
        std::vector<std::unique_ptr<Sphere> > objects_;
        std::optional<Light> light_;
    };

    World default_world();
}
