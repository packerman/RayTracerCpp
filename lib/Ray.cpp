#include "Ray.h"

namespace rt {
    Ray ray(const Point& origin, const Vector& direction) {
        return {origin, direction};
    }

    std::ostream& operator<<(std::ostream& os, const Ray& obj) {
        return os
               << "origin: " << obj.origin_
               << " direction: " << obj.direction_;
    }
}
