#include <gtest/gtest.h>

#include "Sphere.h"

#include "Common.h"
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

    class RayTransformedSphereIntersectionTest
            : public ::testing::TestWithParam<std::tuple<Transformation, std::vector<double> > > {
    };

    TEST_P(RayTransformedSphereIntersectionTest, RayTransformedSphereIntersection) {
        constexpr auto r = Ray{point(0, 0, -5), vector(0, 0, 1)};
        auto [m, expected] = GetParam();
        auto s = Sphere();

        s.set_transform(m);
        const auto xs = s.intersect(r);

        EXPECT_EQ(xs.size(), expected.size());
        for (auto i = 0; i < expected.size(); ++i) {
            EXPECT_EQ(xs[i].t(), expected[i]);
        }
    }

    INSTANTIATE_TEST_SUITE_P(
        RayTransformedSphereIntersectionSuite,
        RayTransformedSphereIntersectionTest,
        ::testing::Values(
            std::make_tuple(scaling(2, 2, 2), std::vector<double>{3, 7}),
            std::make_tuple(translation(5, 0, 0), std::vector<double>{})
        ));

    class SphereNormalTest : public ::testing::TestWithParam<std::tuple<Point, Vector> > {
    };

    TEST_P(SphereNormalTest, SphereNormal) {
        auto [input_point, expected_normal] = GetParam();

        Sphere s{};

        const auto n = s.normal_at(input_point);

        EXPECT_EQ(n, expected_normal);
    }

    INSTANTIATE_TEST_SUITE_P(
        SphereNormalSuite,
        SphereNormalTest,
        ::testing::Values(
            std::make_tuple(point(1, 0, 0), vector(1, 0, 0)),
            std::make_tuple(point(0, 1, 0), vector(0, 1, 0)),
            std::make_tuple(point(0, 0, 1), vector(0, 0, 1)),
            std::make_tuple(
                point(std::numbers::sqrt3 / 3, std::numbers::sqrt3 / 3, std::numbers::sqrt3 / 3),
                vector(std::numbers::sqrt3 / 3, std::numbers::sqrt3 / 3, std::numbers::sqrt3 / 3)
            )
        ));

    TEST(SphereTest, NormalizedNormal) {
        Sphere s{};

        const auto n = s.normal_at(
            point(std::numbers::sqrt3 / 3, std::numbers::sqrt3 / 3, std::numbers::sqrt3 / 3));

        EXPECT_EQ(n, n.normalize());
    }

    class SphereTransformedNormalTest
            : public ::testing::TestWithParam<std::tuple<Transformation, Point, Vector> > {
    };

    TEST_P(SphereTransformedNormalTest, SphereTransformedNormal) {
        auto [transform, point, expected_normal] = GetParam();

        Sphere s{};
        s.set_transform(transform);

        const auto n = s.normal_at(point);

        EXPECT_TRUE(approx_equals(n, expected_normal, 1e-5));
    }

    INSTANTIATE_TEST_SUITE_P(
        SphereTransformedNormalSuite,
        SphereTransformedNormalTest,
        ::testing::Values(
            std::make_tuple(
                translation(0, 1, 0),
                point(0, 1.70711, -0.70711),
                vector(0, 0.70711, -0.70711)),
            std::make_tuple(
                scaling(1, 0.5, 1) * rotation_z(std::numbers::pi / 5),
                point(0, std::numbers::sqrt2 / 2, - std::numbers::sqrt2 / 2),
                vector(0, 0.97014, -0.24254))
        ));
}
