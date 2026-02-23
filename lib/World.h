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

        [[nodiscard]] const std::vector<std::unique_ptr<Light> > &lights() const {
            return lights_;
        }

        [[nodiscard]] const std::unique_ptr<Sphere> &object(const std::size_t index) const {
            return objects_[index];
        }

        [[nodiscard]] const std::unique_ptr<Light> &light(const std::size_t index) const {
            return lights_[index];
        }

        void add_object(std::unique_ptr<Sphere> object) {
            objects_.emplace_back(std::move(object));
        }

        void add_light(std::unique_ptr<Light> light) {
            lights_.emplace_back(std::move(light));
        }

        void remove_light(const int index) {
            lights_.erase(lights_.begin() + index);
        }

        [[nodiscard]] Intersections intersect(const Ray &ray) const;

        [[nodiscard]] Color shade_hit(const Computations &comps) const;

        [[nodiscard]] Color color_at(const Ray &ray) const;

    private:
        std::vector<std::unique_ptr<Sphere> > objects_;
        std::vector<std::unique_ptr<Light> > lights_;
    };

    World default_world();
}
