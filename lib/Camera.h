#pragma once

#include "Transformation.h"
#include "Ray.h"

namespace rt {
    class Camera {
    public:
        [[nodiscard]] Camera(std::size_t h_size, std::size_t v_size, double field_of_view);

        [[nodiscard]] std::size_t h_size() const {
            return h_size_;
        }

        [[nodiscard]] std::size_t v_size() const {
            return v_size_;
        }

        [[nodiscard]] double field_of_view() const {
            return field_of_view_;
        }

        [[nodiscard]] const Transformation &transform() const {
            return transform_;
        }

        void set_transform(const Transformation &transform) {
            transform_ = transform;
            inversed_transform_ = transform.inverse();
        }

        [[nodiscard]] const Transformation &inversed_transform() const {
            return inversed_transform_;
        }

        [[nodiscard]] double pixel_size() const {
            return pixel_size_;
        }

        [[nodiscard]] double half_width() const {
            return half_width_;
        }

        [[nodiscard]] double half_height() const {
            return half_height_;
        }

        [[nodiscard]] Ray ray_for_pixel(std::size_t px, std::size_t py) const;

    private:
        std::size_t h_size_;
        std::size_t v_size_;
        double field_of_view_;
        Transformation transform_{Transformation::identity()};
        Transformation inversed_transform_{Transformation::identity()};
        double pixel_size_;
        double half_width_;
        double half_height_;
    };
}
