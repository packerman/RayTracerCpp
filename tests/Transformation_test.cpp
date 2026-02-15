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
}
