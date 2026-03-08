#pragma once
#include <memory>
#include <ostream>

#include "Pattern.h"
#include "Tuple.h"

namespace rt {
    struct Light {
        Point position;
        Color intensity;

        friend bool operator==(const Light &lhs, const Light &rhs) {
            return std::tie(lhs.position, lhs.intensity) == std::tie(rhs.position, rhs.intensity);
        }

        friend bool operator!=(const Light &lhs, const Light &rhs) {
            return !(lhs == rhs);
        }

        friend std::ostream &operator<<(std::ostream &os, const Light &obj) {
            return os
                   << "position: " << obj.position
                   << " intensity: " << obj.intensity;
        }
    };

    std::unique_ptr<Light> point_light(const Point &position, const Color &intensity);

    struct Material {
        Color color{rt::color(1, 1, 1)};
        double ambient{0.1};
        double diffuse{0.9};
        double specular{0.9};
        double shininess{200.0};
        std::shared_ptr<Pattern> pattern{nullptr};
        double reflective{0};
        double transparency{0};
        double refractive_index{1};

        [[nodiscard]] Color lighting(const Shape &shape, const Light &light, const Point &point, const Vector &eye_v,
                                     const Vector &normal_v, bool in_shadow = false) const;;

        friend bool operator==(const Material &lhs, const Material &rhs) {
            return std::tie(lhs.color, lhs.ambient, lhs.diffuse, lhs.specular, lhs.shininess) == std::tie(
                       rhs.color, rhs.ambient, rhs.diffuse, rhs.specular, rhs.shininess);
        }

        friend bool operator!=(const Material &lhs, const Material &rhs) {
            return !(lhs == rhs);
        }

        friend std::ostream &operator<<(std::ostream &os, const Material &obj);
    };

    constexpr double vacuum_refractive_index{1};
    constexpr double air_refractive_index{1.00029};
    constexpr double water_refractive_index{1.333};
    constexpr double glass_refractive_index{1.52};
    constexpr double diamond_refractive_index{2.417};
}
