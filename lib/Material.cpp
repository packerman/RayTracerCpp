#include "Material.h"

#include <cmath>

namespace rt {
    std::unique_ptr<Light> point_light(const Point &position, const Color &intensity) {
        return std::make_unique<Light>(position, intensity);
    }

    Color Material::lighting(const Light &light, const Point &point, const Vector &eye_v,
                             const Vector &normal_v, const bool in_shadow) const {
        const auto effective_color = this->color * light.intensity;
        const auto light_v = (light.position - point).normalize();

        const auto ambient_color = effective_color * this->ambient;

        if (in_shadow) {
            return ambient_color;
        }

        auto diffuse_color = black;
        auto specular_color = black;
        if (const auto light_dot_normal = light_v.dot(normal_v); light_dot_normal >= 0) {
            diffuse_color = effective_color * this->diffuse * light_dot_normal;

            const auto reflect_v = -light_v.reflect(normal_v);
            if (const auto reflect_dot_eye = reflect_v.dot(eye_v); reflect_dot_eye > 0) {
                const auto factor = std::pow(reflect_dot_eye, this->shininess);
                specular_color = light.intensity * this->specular * factor;
            }
        }

        return ambient_color + diffuse_color + specular_color;
    }

    std::ostream &operator<<(std::ostream &os, const Material &obj) {
        return os
               << "color: " << obj.color
               << " ambient: " << obj.ambient
               << " diffuse: " << obj.diffuse
               << " specular: " << obj.specular
               << " shininess: " << obj.shininess;
    }
}
