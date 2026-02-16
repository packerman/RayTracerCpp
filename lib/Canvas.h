#pragma once

#include "Tuple.h"
#include <vector>

namespace rt {
    class Canvas {
    public:
        Canvas(const std::size_t width, const std::size_t height,
               const Color &background_color = color(0, 0, 0)) : width_(width), height_(height),
                                                                 pixels_(height, std::vector(width, background_color)) {
        }

        [[nodiscard]] std::size_t width() const {
            return width_;
        }

        [[nodiscard]] std::size_t height() const {
            return height_;
        }

        [[nodiscard]] const Color &pixel_at(const std::size_t i, const std::size_t j) const {
            return pixels_[j][i];
        }

        void write_pixel(const std::size_t i, const std::size_t j, const Color &color) {
            pixels_[j][i] = color;
        }

        void to_ppm(std::ostream &os) const;;

    private:
        std::size_t width_, height_;
        std::vector<std::vector<Color> > pixels_{};
    };
} // rt
