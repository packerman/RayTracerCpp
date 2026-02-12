#include "Canvas.h"
#include <limits>
#include <algorithm>
#include <cmath>
#include <string>

namespace rt {

    void Canvas::to_ppm(std::ostream &os) const {
        static constexpr auto max_val = static_cast<int>(std::numeric_limits<unsigned char>::max());

        os << "P3\n" << width_ << " " << height_ << "\n" << max_val << "\n";

        auto to_ppm_value = [](const double value) {
            return std::to_string(std::lround(max_val * std::clamp(value, 0.0, 1.0)));
        };

        for (auto row = 0; row < height_; ++row) {
            for (auto column = 0; column < width_; ++column) {
                auto& c = pixel_at(column, row);
                if (column > 0) {
                    os << " ";
                }
                os << to_ppm_value(c.red()) << " "
                        << to_ppm_value(c.green()) << " "
                        << to_ppm_value(c.blue());
            }
            os << "\n";
        }
    }
} // rt
