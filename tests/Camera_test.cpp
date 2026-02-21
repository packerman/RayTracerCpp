#include <gtest/gtest.h>
#include "Camera.h"

#include "Common.h"

namespace rt {
    TEST(CameraTest, Create) {
        constexpr std::size_t h_size = 160;
        constexpr std::size_t v_size = 120;
        constexpr auto field_of_view = std::numbers::pi / 2;

        const auto c = Camera{h_size, v_size, field_of_view};

        EXPECT_EQ(c.h_size(), 160);
        EXPECT_EQ(c.v_size(), 120);
        EXPECT_EQ(c.field_of_view(), std::numbers::pi / 2);
        EXPECT_EQ(c.transform(), Transformation::identity());
    }

    class PixelSizeTest : public ::testing::TestWithParam<std::tuple<Camera, double> > {
    };

    TEST_P(PixelSizeTest, PixelSize) {
        auto [camera, pixel_size] = GetParam();

        EXPECT_TRUE(approx_equals(camera.pixel_size(), pixel_size));
    }

    INSTANTIATE_TEST_SUITE_P(
        PixelSizeSuite,
        PixelSizeTest,
        ::testing::Values(
            std::make_tuple(Camera{200, 125, std::numbers::pi / 2}, 0.01),
            std::make_tuple(Camera{125, 200, std::numbers::pi / 2}, 0.01)
        ));

    class RayForPixelTest
            : public ::testing::TestWithParam<std::tuple<int, int, Transformation, Ray> > {
    };

    TEST_P(RayForPixelTest, RayForPixel) {
        auto [i, j, transform, expected] = GetParam();

        Camera c{201, 101, std::numbers::pi / 2};
        c.set_transform(transform);

        const auto r = c.ray_for_pixel(i, j);

        EXPECT_EQ(r.origin(), expected.origin());
        EXPECT_TRUE(approx_equals(r.direction(), expected.direction(), 1e-5));
    }

    INSTANTIATE_TEST_SUITE_P(
        RayForPixelSuite,
        RayForPixelTest,
        ::testing::Values(
            std::make_tuple(100, 50, Transformation::identity(),
                Ray{point(0, 0, 0), vector(0, 0, -1)}),
            std::make_tuple(0, 0, Transformation::identity(),
                Ray{point(0, 0, 0), vector(0.66519, 0.33259, -0.66851)}),
            std::make_tuple(100, 50, rotation_y(std::numbers::pi / 4) * translation(0, -2, 5),
                Ray{point(0, 2, -5), vector(std::numbers::sqrt2 / 2, 0, -std::numbers::sqrt2 / 2)})
        ));
}
