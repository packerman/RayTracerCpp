#include <Material.h>
#include <gtest/gtest.h>

namespace rt {

    TEST(LightTest, CreateLight) {
        constexpr auto intensity = color(1, 1, 1);
        constexpr auto position = point(0, 0, 0);

        constexpr Light light{position, intensity};

        EXPECT_EQ(light.position, position);
        EXPECT_EQ(light.intensity, intensity);
    }

    TEST(MaterialTest, CreateMaterial) {
        Material m{};

        EXPECT_EQ(m.color, color(1, 1, 1));
        EXPECT_EQ(m.ambient, 0.1);
        EXPECT_EQ(m.diffuse, 0.9);
        EXPECT_EQ(m.specular, 0.9);
        EXPECT_EQ(m.shininess, 200.0);
    }
}