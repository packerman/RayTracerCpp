#include <gtest/gtest.h>

#include "Shape.h"

#include "Common.h"
#include "Ray.h"
#include "Tuple.h"
#include "Intersection.h"
#include "Transformation.h"

namespace rt {
    struct TestShape : Shape {
        std::vector<Intersection> local_intersect(const Ray &ray) override {
            saved_ray = ray;
            return {};
        }

        Vector local_normal_at(const Point &point) override {
            return vector(point.x, point.y, point.z);
        }

        Ray saved_ray{point(0, 0, 0), vector(0, 0, 0)};
    };

    std::unique_ptr<TestShape> test_shape() {
        return std::make_unique<TestShape>();
    }

    TEST(ShapeTest, DefaultTransformation) {
        const auto s = test_shape();
        EXPECT_EQ(s->transform(), Transformation::identity());
        EXPECT_EQ(s->inversed_transform(), Transformation::identity());
    }

    TEST(ShapeTest, ChangeTransformation) {
        const auto s = test_shape();
        constexpr auto t = translation(2, 3, 4);
        constexpr auto t_inv = translation(-2, -3, -4);

        s->set_transform(t);

        EXPECT_EQ(s->transform(), t);
        EXPECT_EQ(s->inversed_transform(), t_inv);
    }

    TEST(ShapeTest, HasMaterial) {
        const auto s = test_shape();

        const auto m = s->material();

        EXPECT_EQ(m, Material{});
    }

    TEST(ShapeTest, SetMaterial) {
        const auto s = test_shape();
        Material m{};
        m.ambient = 1;

        s->set_material(m);

        EXPECT_EQ(s->material(), m);
    }

    class RayTransformedShapeIntersectionTest
            : public ::testing::TestWithParam<std::tuple<Transformation, Ray> > {
    };

    TEST_P(RayTransformedShapeIntersectionTest, RayTransformedShapeIntersection) {
        constexpr auto r = Ray{point(0, 0, -5), vector(0, 0, 1)};
        auto [m, expected] = GetParam();
        auto s = test_shape();

        s->set_transform(m);
        const auto xs = s->intersect(r);

        EXPECT_EQ(s->saved_ray.origin(), expected.origin());
        EXPECT_EQ(s->saved_ray.direction(), expected.direction());
    }

    INSTANTIATE_TEST_SUITE_P(
        RayTransformedShapeIntersectionSuite,
        RayTransformedShapeIntersectionTest,
        ::testing::Values(
            std::make_tuple(scaling(2, 2, 2), Ray(point(0, 0, -2.5), vector(0, 0, 0.5))),
            std::make_tuple(translation(5, 0, 0), Ray(point(-5, 0, -5), vector(0, 0, 1)))
        ));

    class ShapeTransformedNormalTest
            : public ::testing::TestWithParam<std::tuple<Transformation, Point, Vector> > {
    };

    TEST_P(ShapeTransformedNormalTest, ShapeTransformedNormal) {
        auto [transform, point, expected_normal] = GetParam();

        auto s = test_shape();
        s->set_transform(transform);

        const auto n = s->normal_at(point);

        EXPECT_TRUE(approx_equals(n, expected_normal, 1e-5));
    }

    INSTANTIATE_TEST_SUITE_P(
        ShapeTransformedNormalSuite,
        ShapeTransformedNormalTest,
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

    class RaySphereIntersectionTest : public ::testing::TestWithParam<std::tuple<Ray, std::vector<double> > > {
    };

    TEST_P(RaySphereIntersectionTest, RaySphereIntersection) {
        auto [ray, expected] = GetParam();
        const auto s = sphere();

        const auto xs = s->local_intersect(ray);

        ASSERT_EQ(xs.size(), expected.size());
        for (auto i = 0; i < expected.size(); ++i) {
            EXPECT_EQ(xs[i].t(), expected[i]);
            EXPECT_EQ(xs[i].object(), s.get());
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
        const auto s = sphere();

        const auto xs = s->local_intersect(ray);

        EXPECT_EQ(xs.size(), 2);
        EXPECT_EQ(xs[0].object(), s.get());
        EXPECT_EQ(xs[1].object(), s.get());
    }

    class SphereNormalTest : public ::testing::TestWithParam<std::tuple<Point, Vector> > {
    };

    TEST_P(SphereNormalTest, SphereNormal) {
        auto [input_point, expected_normal] = GetParam();

        const auto s = sphere();

        const auto n = s->local_normal_at(input_point);

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
        const auto s = sphere();

        const auto n = s->local_normal_at(
            point(std::numbers::sqrt3 / 3, std::numbers::sqrt3 / 3, std::numbers::sqrt3 / 3));

        EXPECT_EQ(n, n.normalize());
    }

    TEST(PlaneTest, PlaneNormalIsConstant) {
        auto p = plane();

        const auto n1 = p->local_normal_at(point(0, 0, 0));
        const auto n2 = p->local_normal_at(point(10, 0, -10));
        const auto n3 = p->local_normal_at(point(-5, 0, 150));

        EXPECT_EQ(n1, vector(0, 1, 0));
        EXPECT_EQ(n2, vector(0, 1, 0));
        EXPECT_EQ(n3, vector(0, 1, 0));
    }

    class RayPlaneIntersectionTest : public ::testing::TestWithParam<std::tuple<Ray, std::vector<double> > > {
    };

    TEST_P(RayPlaneIntersectionTest, RaySphereIntersection) {
        auto [ray, expected] = GetParam();
        const auto p = plane();

        const auto xs = p->local_intersect(ray);

        ASSERT_EQ(xs.size(), expected.size());
        for (auto i = 0; i < expected.size(); ++i) {
            EXPECT_EQ(xs[i].t(), expected[i]);
            EXPECT_EQ(xs[i].object(), p.get());
        }
    }

    INSTANTIATE_TEST_SUITE_P(
        RayPlaneIntersectionSuite,
        RayPlaneIntersectionTest,
        ::testing::Values(
            std::make_tuple(Ray(point(0, 10, 0), vector(0, 0, 1)), std::vector<double>{}),
            std::make_tuple(Ray(point(0, 0, 0), vector(0, 0, 1)), std::vector<double>{}),
            std::make_tuple(Ray(point(0, 1, 0), vector(0, -1, 0)), std::vector{1.0}),
            std::make_tuple(Ray(point(0, -1, 0), vector(0, 1, 0)), std::vector{1.0})
        ));
}
