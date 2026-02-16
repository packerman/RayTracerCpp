#include "Canvas.h"
#include <limits>
#include <algorithm>
#include <cmath>
#include <string>
#include <fstream>
#include <iostream>

namespace rt {

    void Canvas::to_ppm(std::ostream &os) const {
        static constexpr auto max_val = static_cast<int>(std::numeric_limits<unsigned char>::max());

        os << "P3\n" << width_ << " " << height_ << "\n" << max_val << "\n";

        auto to_ppm_value = [](const double value) {
            return std::to_string(std::lround(max_val * std::clamp(value, 0.0, 1.0)));
        };

        std::size_t line_length = 0;

        auto write_string = [&](const std::string& s){
            if (line_length + s.length() + 1 > 70) {
                os << "\n";
                line_length = 0;
            }
            if (line_length > 0) {
                os << " ";
                line_length++;
            }
            os << s;
            line_length += s.length();
        };

        for (auto row = 0; row < height_; ++row) {
            for (auto column = 0; column < width_; ++column) {
                auto& c = pixel_at(column, row);
                write_string(to_ppm_value(c.red()));
                write_string(to_ppm_value(c.green()));
                write_string(to_ppm_value(c.blue()));
            }
            os << "\n";
            line_length = 0;
        }
    }

    void save_to_file(const std::string &filename, const Canvas &canvas) {
        std::ofstream outfile(filename);
        if (outfile) {
            canvas.to_ppm(outfile);
        } else {
            std::cerr << "Cannot open file " << filename << "\n";
        }
    }
} // rt
