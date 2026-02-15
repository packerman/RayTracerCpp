#include <gtest/gtest.h>

#include <Transformation.h>

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
}
