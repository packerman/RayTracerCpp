#include <gtest/gtest.h>
#include <Canvas.h>
#include "Common.h"

namespace rt {
    TEST(CanvasTest, CreatingCanvas) {
        const auto c = Canvas{10, 20};
        EXPECT_EQ(c.width(), 10);
        EXPECT_EQ(c.height(), 20);
        for (auto i = 0; i < c.width(); ++i) {
            for (auto j = 0; j < c.height(); ++j) {
                EXPECT_EQ(c.pixel_at(i, j), color(0, 0, 0));
            }
        }
    }

    TEST(CanvasTest, CreatingCanvasWithBackgroundColor) {
        const auto c = Canvas{10, 20, color(1, 0, 0)};
        EXPECT_EQ(c.width(), 10);
        EXPECT_EQ(c.height(), 20);
        for (auto i = 0; i < c.width(); ++i) {
            for (auto j = 0; j < c.height(); ++j) {
                EXPECT_EQ(c.pixel_at(i, j), color(1, 0, 0));
            }
        }
    }

    TEST(CanvasTest, WritingPixels) {
        auto c = Canvas{10, 20};
        const auto red = color(1, 0, 0);
        c.write_pixel(2, 3, red);
        EXPECT_EQ(c.pixel_at(2, 3), red);
    }

    std::string to_ppm_string(const Canvas &c) {
        std::stringstream ss;
        c.to_ppm(ss);
        return ss.str();
    }

    TEST(CanvasTest, PPMHeader) {
        const auto c = Canvas{5, 3};
        const auto ppm_lines = string_lines(to_ppm_string(c));
        EXPECT_EQ(ppm_lines[0], "P3");
        EXPECT_EQ(ppm_lines[1], "5 3");
        EXPECT_EQ(ppm_lines[2], "255");
    }

    TEST(CanvasTest, PixelData) {
        auto c = Canvas{5, 3};
        const auto c1 = color(1.5, 0, 0);
        const auto c2 = color(0, 0.5, 0);
        const auto c3 = color(-0.5, 0, 1);

        c.write_pixel(0, 0, c1);
        c.write_pixel(2, 1, c2);
        c.write_pixel(4, 2, c3);
        const auto ppm_lines = string_lines(to_ppm_string(c));

        EXPECT_EQ(ppm_lines[3],
            "255 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
        EXPECT_EQ(ppm_lines[4],
            "0 0 0 0 0 0 0 128 0 0 0 0 0 0 0");
        EXPECT_EQ(ppm_lines[5],
            "0 0 0 0 0 0 0 0 0 0 0 0 0 0 255");
    }

    TEST(CanvasTest, SplittingLongLines) {
        auto c = Canvas{10, 2};

        for (auto i = 0; i < c.width(); ++i) {
            for (auto j = 0; j < c.height(); ++j) {
                c.write_pixel(i, j, color(1, 0.8, 0.6));
            }
        }
        const auto ppm_lines = string_lines(to_ppm_string(c));

        EXPECT_EQ(ppm_lines[3],
            "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
        EXPECT_EQ(ppm_lines[4],
            "153 255 204 153 255 204 153 255 204 153 255 204 153");
        EXPECT_EQ(ppm_lines[5],
            "255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
        EXPECT_EQ(ppm_lines[6],
            "153 255 204 153 255 204 153 255 204 153 255 204 153");
    }

    TEST(CanvasTest, NewlineAtThePPMEnd) {
        const auto c = Canvas{5, 3};

        const auto ppm = to_ppm_string(c);

        EXPECT_EQ(ppm.back(), '\n');
    }
}
