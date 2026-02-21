#pragma once
#include <ostream>

#include "Tuple.h"

namespace rt {
    struct Light {
        Point position;
        Color intensity;
    };

    struct Material {
        Color color{rt::color(1, 1, 1)};
        double ambient{0.1};
        double diffuse{0.9};
        double specular{0.9};
        double shininess{200.0};

        friend bool operator==(const Material &lhs, const Material &rhs) {
            return std::tie(lhs.color, lhs.ambient, lhs.diffuse, lhs.specular, lhs.shininess) == std::tie(
                       rhs.color, rhs.ambient, rhs.diffuse, rhs.specular, rhs.shininess);
        }

        friend bool operator!=(const Material &lhs, const Material &rhs) {
            return !(lhs == rhs);
        }

        friend std::ostream & operator<<(std::ostream &os, const Material &obj) {
            return os
                   << "color: " << obj.color
                   << " ambient: " << obj.ambient
                   << " diffuse: " << obj.diffuse
                   << " specular: " << obj.specular
                   << " shininess: " << obj.shininess;
        }
    };
}
