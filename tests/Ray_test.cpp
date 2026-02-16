#include <gtest/gtest.h>

#include <Ray.h>

namespace rt {

    TEST(RayTest, CreateRay) {
        constexpr auto origin = point(1, 2, 3);
        constexpr auto direction = point(4, 5, 6);

        constexpr auto r = Ray(origin, direction);

        EXPECT_EQ(r.origin(), origin);
        EXPECT_EQ(r.direction(), direction);
    }

    TEST(RayTest, ComputePosition) {
        constexpr auto r = Ray(point(2, 3, 4), vector(1, 0, 0));

        EXPECT_EQ(r.position(0), point(2, 3, 4));
        EXPECT_EQ(r.position(1), point(3, 3, 4));
        EXPECT_EQ(r.position(-1), point(1, 3, 4));
        EXPECT_EQ(r.position(2.5), point(4.5, 3, 4));
    }
}