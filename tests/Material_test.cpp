#include <Material.h>
#include <gtest/gtest.h>

#include "Common.h"

namespace rt {
    TEST(LightTest, CreateLight) {
        constexpr auto intensity = color(1, 1, 1);
        constexpr auto position = point(0, 0, 0);

        constexpr Light light{position, intensity};

        EXPECT_EQ(light.position, position);
        EXPECT_EQ(light.intensity, intensity);
    }

    TEST(MaterialTest, CreateMaterial) {
        constexpr Material m{};

        EXPECT_EQ(m.color, color(1, 1, 1));
        EXPECT_EQ(m.ambient, 0.1);
        EXPECT_EQ(m.diffuse, 0.9);
        EXPECT_EQ(m.specular, 0.9);
        EXPECT_EQ(m.shininess, 200.0);
    }

    class LightingTest
            : public ::testing::TestWithParam<std::tuple<Vector, Vector, Light, Color> > {
    protected:
        Material m_;
        Point position_{point(0, 0, 0)};
    };

    TEST_P(LightingTest, Lighting) {
        auto [eye_v, normal_v, light, expected] = GetParam();

        const auto result = m_.lighting(light, position_, eye_v, normal_v);

        EXPECT_TRUE(approx_equals(result, expected, 1e-5));
    }

    INSTANTIATE_TEST_SUITE_P(
        LightingSuite,
        LightingTest,
        ::testing::Values(
            std::make_tuple(
                vector(0, 0, -1),
                vector(0, 0, -1),
                Light{point(0, 0, -10), color(1, 1, 1)},
                color(1.9, 1.9, 1.9)
            ),
            std::make_tuple(
                vector(0, std::numbers::sqrt2 / 2, - std::numbers::sqrt2 / 2),
                vector(0, 0, -1),
                Light{point(0, 0, -10), color(1, 1, 1)},
                color(1.0, 1.0, 1.0)
            ),
            std::make_tuple(
                vector(0, 0, -1),
                vector(0, 0, -1),
                Light{point(0, 10, -10), color(1, 1, 1)},
                color(0.7364, 0.7364, 0.7364)
            ),
            std::make_tuple(
                vector(0, - std::numbers::sqrt2 / 2, - std::numbers::sqrt2 / 2),
                vector(0, 0, -1),
                Light{point(0, 10, -10), color(1, 1, 1)},
                color(1.6364, 1.6364, 1.6364)
            ),
            std::make_tuple(
                vector(0, 0, -1),
                vector(0, 0, -1),
                Light{point(0, 0, 10), color(1, 1, 1)},
                color(0.1, 0.1, 0.1)
            )
        ));
}
