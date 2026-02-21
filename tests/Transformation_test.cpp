#include <gtest/gtest.h>
#include <numbers>

#include <Transformation.h>
#include "Common.h"

namespace rt {
    TEST(TransformationTest, Translation) {
        constexpr auto transform = translation(5, -3, 2);
        constexpr auto p = point(-3, 4, 5);

        EXPECT_EQ(transform * p, point(2, 1, 7));
    }

    TEST(TransformationTest, TranslationInverse) {
        constexpr auto transform = translation(5, -3, 2);
        const auto inv = transform.inverse();
        constexpr auto p = point(-3, 4, 5);

        EXPECT_EQ(inv * p, point(-8, 7, 3));
    }

    TEST(TransformationTest, VectorTranslation) {
        constexpr auto transform = translation(5, -3, 2);
        constexpr auto p = point(-3, 4, 5);

        EXPECT_EQ(transform * p, point(2, 1, 7));
    }

    TEST(TransformationTest, PointScaling) {
        constexpr auto transform = scaling(2, 3, 4);
        constexpr auto p = point(-4, 6, 8);

        EXPECT_EQ(transform * p, point(-8, 18, 32));
    }

    TEST(TransformationTest, VectorScaling) {
        constexpr auto transform = scaling(2, 3, 4);
        constexpr auto v = vector(-4, 6, 8);

        EXPECT_EQ(transform * v, vector(-8, 18, 32));
    }

    TEST(TransformationTest, ScalingInverse) {
        constexpr auto transform = scaling(2, 3, 4);
        const auto inv = transform.inverse();
        constexpr auto v = vector(-4, 6, 8);

        EXPECT_EQ(inv * v, vector(-2, 2, 2));
    }

    TEST(TransformationTest, Reflection) {
        constexpr auto transform = scaling(-1, 1, 1);
        constexpr auto p = point(2, 3, 4);

        EXPECT_EQ(transform * p, point(-2, 3, 4));
    }

    TEST(TransformationTest, RotationX) {
        constexpr auto p = point(0, 1, 0);
        const auto half_quarter = rotation_x(std::numbers::pi / 4);
        const auto full_quarter = rotation_x(std::numbers::pi / 2);

        EXPECT_TRUE(approx_equals(half_quarter * p,point(0, std::numbers::sqrt2 / 2, std::numbers::sqrt2 / 2)));
        EXPECT_TRUE(approx_equals(full_quarter * p, point(0, 0, 1)));
    }

    TEST(TransformationTest, RotationXInverse) {
        constexpr auto p = point(0, 1, 0);
        const auto half_quarter = rotation_x(std::numbers::pi / 4);
        const auto inv = half_quarter.inverse();

        EXPECT_TRUE(approx_equals(inv * p, point(0, std::numbers::sqrt2 / 2, - std::numbers::sqrt2 / 2)));
    }

    TEST(TransformationTest, RotationY) {
        constexpr auto p = point(0, 0, 1);
        const auto half_quarter = rotation_y(std::numbers::pi / 4);
        const auto full_quarter = rotation_y(std::numbers::pi / 2);

        EXPECT_TRUE(approx_equals(half_quarter * p,point(std::numbers::sqrt2 / 2, 0, std::numbers::sqrt2 / 2)));
        EXPECT_TRUE(approx_equals(full_quarter * p, point(1, 0, 0)));
    }

    TEST(TransformationTest, RotationZ) {
        constexpr auto p = point(0, 1, 0);
        const auto half_quarter = rotation_z(std::numbers::pi / 4);
        const auto full_quarter = rotation_z(std::numbers::pi / 2);

        EXPECT_TRUE(approx_equals(half_quarter * p,point(-std::numbers::sqrt2 / 2, std::numbers::sqrt2 / 2, 0)));
        EXPECT_TRUE(approx_equals(full_quarter * p, point(-1, 0, 0)));
    }

    class ShearingTransformationTest : public ::testing::TestWithParam<std::tuple<Transformation, Point, Point> > {
    };

    TEST_P(ShearingTransformationTest, ShearingTransformation) {
        auto [transform, p, expected] = GetParam();
        EXPECT_EQ(transform * p, expected);
    }

    INSTANTIATE_TEST_SUITE_P(
        ShearingTransformationSuite,
        ShearingTransformationTest,
        ::testing::Values(
            std::make_tuple(shearing(1, 0, 0, 0, 0, 0), point(2, 3, 4), point(5, 3, 4)),
            std::make_tuple(shearing(0, 1, 0, 0, 0, 0), point(2, 3, 4), point(6, 3, 4)),
            std::make_tuple(shearing(0, 0, 1, 0, 0, 0), point(2, 3, 4), point(2, 5, 4)),
            std::make_tuple(shearing(0, 0, 0, 1, 0, 0), point(2, 3, 4), point(2, 7, 4)),
            std::make_tuple(shearing(0, 0, 0, 0, 1, 0), point(2, 3, 4), point(2, 3, 6)),
            std::make_tuple(shearing(0, 0, 0, 0, 0, 1), point(2, 3, 4), point(2, 3, 7))
        ));

    TEST(TransformationTest, TransformationSequence) {
        constexpr auto p = point(1, 0, 1);
        const auto a = rotation_x(std::numbers::pi / 2);
        constexpr auto b = scaling(5, 5, 5);
        constexpr auto c = translation(10, 5, 7);

        const auto p2 = a * p;
        EXPECT_TRUE(approx_equals(p2, point(1, -1, 0)));

        const auto p3 = b * p2;
        EXPECT_TRUE(approx_equals(p3, point(5, -5, 0), 2*machine_epsilon));

        const auto p4 = c * p3;
        EXPECT_EQ(p4, point(15, 0, 7));
    }

    TEST(TransformationTest, ChainTransformation) {
        constexpr auto p = point(1, 0, 1);
        const auto a = rotation_x(std::numbers::pi / 2);
        constexpr auto b = scaling(5, 5, 5);
        constexpr auto c = translation(10, 5, 7);

        const auto t = c * b * a;
        EXPECT_EQ(t * p, point(15, 0, 7));
    }

    TEST(TransformationTest, ChainTransformationFluentApi) {
        const auto t = Transformation::identity()
                .then(rotation_x(std::numbers::pi / 2))
                .then(scaling(5, 5, 5))
                .then(translation(10, 5, 7));
        constexpr auto p = point(1, 0, 1);

        EXPECT_EQ(t * p, point(15, 0, 7));
    }

    class ViewTransformationTest
            : public ::testing::TestWithParam<std::tuple<Point, Point, Vector, Transformation> > {
    };

    TEST_P(ViewTransformationTest, ViewTransformation) {
        auto [from, to, up, expected] = GetParam();

        const auto t = view_transform(from, to, up);

        EXPECT_TRUE(approx_equals(t, expected, 1e-5));
    }

    INSTANTIATE_TEST_SUITE_P(
        ViewTransformationSuite,
        ViewTransformationTest,
        ::testing::Values(
            std::make_tuple(
                point(0, 0, 0), point(0, 0, -1), vector(0, 1, 0),
                Transformation::identity()
            ),
            std::make_tuple(
                point(0, 0, 0), point(0, 0, 1), vector(0, 1, 0),
                scaling(-1, 1, -1)
            ),
            std::make_tuple(
                point(0, 0, 8), point(0, 0, 0), vector(0, 1, 0),
                translation(0, 0, -8)
            ),
            std::make_tuple(
                point(1, 3, 2), point(4, -2, 8), vector(1, 1, 0),
                Transformation({
                    -0.50709, 0.50709, 0.67612, -2.36643,
                    0.76772, 0.60609, 0.12122, -2.82843,
                    -0.35857, 0.59761, -0.71714, 0.0,
                    0.0, 0.0, 0.0, 1.0
                })
            )
        ));
}
