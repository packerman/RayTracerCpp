#include <gtest/gtest.h>

#include "Sphere.h"
#include "Ray.h"
#include "Tuple.h"
#include "Intersection.h"
#include "Transformation.h"

namespace rt {
    class RaySphereIntersectionTest : public ::testing::TestWithParam<std::tuple<Ray, std::vector<double> > > {
    };

    TEST_P(RaySphereIntersectionTest, RaySphereIntersection) {
        auto [ray, expected] = GetParam();
        auto s = Sphere();

        const auto xs = s.intersect(ray);

        EXPECT_EQ(xs.size(), expected.size());
        for (auto i = 0; i < expected.size(); ++i) {
            EXPECT_EQ(xs[i].t(), expected[i]);
            EXPECT_EQ(xs[i].object(), &s);
        }
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

    TEST(SphereTest, SetObjectInIntersection) {
        constexpr Ray ray{point(0, 0, -5), vector(0, 0, 1)};
        Sphere s{};

        const auto xs = s.intersect(ray);

        EXPECT_EQ(xs.size(), 2);
        EXPECT_EQ(xs[0].object(), &s);
        EXPECT_EQ(xs[1].object(), &s);
    }

    TEST(SphereTest, DefaultTransformation) {
        constexpr Sphere s{};
        EXPECT_EQ(s.transform(), Transformation::identity());
    }

    TEST(SphereTest, ChangeTransformation) {
        Sphere s{};
        constexpr auto t = translation(2, 3, 4);

        s.set_transform(t);

        EXPECT_EQ(s.transform(), t);
    }
}
