#include "Camera.h"
#include <cmath>

namespace rt {
    Camera::Camera(const std::size_t h_size, const std::size_t v_size, const double field_of_view)
        : h_size_(h_size),
          v_size_(v_size),
          field_of_view_(field_of_view) {
        const auto half_view = std::tan(field_of_view_ / 2);
        if (auto aspect = static_cast<double>(h_size) / static_cast<double>(v_size); aspect >= 1) {
            half_width_ = half_view;
            half_height_ = half_view / aspect;
        } else {
            half_width_ = half_view * aspect;
            half_height_ = half_view;
        }

        pixel_size_ = (half_width_ * 2) / static_cast<double>(h_size_);
    }

    Ray Camera::ray_for_pixel(const std::size_t px, const std::size_t py) const {
        const auto x_offset = (static_cast<double>(px) + 0.5) * pixel_size();
        const auto y_offset = (static_cast<double>(py) + 0.5) * pixel_size();

        const auto world_x = half_width() - x_offset;
        const auto world_y = half_height() - y_offset;

        const auto pixel = inversed_transform() * point(world_x, world_y, -1);
        const auto origin = inversed_transform() * point(0, 0, 0);
        const auto direction = (pixel - origin).normalize();

        const Ray ray{origin, direction};
        return ray;
    }

    Canvas Camera::render(const World &world) const {
        auto image = Canvas{h_size(), v_size()};

        for (std::size_t y = 0; y < v_size(); ++y) {
            for (std::size_t x = 0; x < h_size(); ++x) {
                auto ray = ray_for_pixel(x, y);
                auto color = world.color_at(ray);
                image.write_pixel(x, y, color);
            }
        }

        return image;
    }
}
