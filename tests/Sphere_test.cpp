#include <gtest/gtest.h>

#include "Sphere.h"
#include "Ray.h"
#include "Tuple.h"

namespace rt {
    class RaySphereIntersectionTest : public ::testing::TestWithParam<std::tuple<Ray, std::vector<double> > > {
    };

    TEST_P(RaySphereIntersectionTest, RaySphereIntersection) {
        auto [ray, expected] = GetParam();
        auto s = Sphere();

        const auto xs = s.intersect(ray);
        EXPECT_EQ(xs, expected);
    }

    INSTANTIATE_TEST_SUITE_P(
        RaySphereIntersectionSuite,
        RaySphereIntersectionTest,
        ::testing::Values(
            std::make_tuple(Ray(point(0, 0, -5), vector(0, 0, 1)), std::vector{4.0, 6.0}),
            std::make_tuple(Ray(point(0, 1, -5), vector(0, 0, 1)), std::vector{5.0, 5.0}),
            std::make_tuple(Ray(point(0, 2, -5), vector(0, 0, 1)), std::vector<double>{}),
            std::make_tuple(Ray(point(0, 0, 0), vector(0, 0, 1)), std::vector{-1.0, 1.0}),
            std::make_tuple(Ray(point(0, 0, 5), vector(0, 0, 1)), std::vector{-6.0, -4.0})
        ));
}
