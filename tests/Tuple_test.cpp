#include <Tuple.h>

#include <cmath>
#include <tuple>
#include <gtest/gtest.h>

#include "Common.h"

namespace rt {
    TEST(TupleTest, CreatingPointTuple) {
        constexpr Tuple a{4.3, -4.2, 3.1, 1.0};
        EXPECT_EQ(a.x, 4.3);
        EXPECT_EQ(a.y, -4.2);
        EXPECT_EQ(a.z, 3.1);
        EXPECT_EQ(a.w, 1.0);
        EXPECT_TRUE(a.is_point());
        EXPECT_FALSE(a.is_vector());
    }

    TEST(TupleTest, CreatingVectorTuple) {
        constexpr Tuple a{4.3, -4.2, 3.1, 0.0};
        EXPECT_EQ(a.x, 4.3);
        EXPECT_EQ(a.y, -4.2);
        EXPECT_EQ(a.z, 3.1);
        EXPECT_EQ(a.w, 0.0);
        EXPECT_FALSE(a.is_point());
        EXPECT_TRUE(a.is_vector());
    }

    TEST(TupleTest, PointMethod) {
        const auto p = point(4, -4, 3);
        EXPECT_EQ(p, Tuple(4, -4, 3, 1));
    }

    TEST(TupleTest, VectorMethod) {
        const auto p = vector(4, -4, 3);
        EXPECT_EQ(p, Tuple(4, -4, 3, 0));
    }

    TEST(TupleTest, AddingTuples) {
        constexpr Tuple a1{3, -2, 5, 1};
        constexpr Tuple a2{-2, 3, 1, 0};
        EXPECT_EQ(a1 + a2, Tuple(1, 1, 6, 1));
    }

    TEST(TupleTest, PlusEqual) {
         Tuple a1{3, -2, 5, 1};
         constexpr Tuple a2{-2, 3, 1, 0};

        a1 += a2;

        EXPECT_EQ(a1, Tuple(1, 1, 6, 1));
    }

    class TupleSubtractTest : public ::testing::TestWithParam<std::tuple<Tuple, Tuple, Tuple> > {
    };

    TEST_P(TupleSubtractTest, SubtractingTuples) {
        auto [a, b, expected] = GetParam();
        EXPECT_EQ(a - b, expected);
    }

    INSTANTIATE_TEST_SUITE_P(
        SubtractSuite,
        TupleSubtractTest,
        ::testing::Values(
            std::make_tuple(point(3, 2, 1), point(5, 6, 7),
                vector(-2, -4, -6)),
            std::make_tuple(point(3, 2, 1), vector(5, 6, 7),
                point(-2, -4, -6)),
            std::make_tuple(vector(3, 2, 1), vector(5, 6, 7),
                vector(-2, -4, -6)),
            std::make_tuple(vector(0, 0, 0), vector(1, -2, 3),
                vector(-1, 2, -3))
        )
    );

    TEST(TupleTest, NegatingTuple) {
        constexpr Tuple a{1, -2, 3, -4};
        EXPECT_EQ(-a, Tuple(-1, 2, -3, 4));
    }

    class TupleMultiplyTest : public ::testing::TestWithParam<std::tuple<Tuple, double, Tuple> > {
    };

    TEST_P(TupleMultiplyTest, MultiplyingTuples) {
        auto [a, s, expected] = GetParam();
        EXPECT_EQ(a * s, expected);
    }

    INSTANTIATE_TEST_SUITE_P(
        MultiplySuite,
        TupleMultiplyTest,
        ::testing::Values(
            std::make_tuple(Tuple(1, -2, 3, -4), 3.5, Tuple(3.5, -7, 10.5, -14)),
            std::make_tuple(Tuple(1, -2, 3, -4), 0.5, Tuple(0.5, -1, 1.5, -2))
        )
    );

    TEST(TupleTest, DividingTuples) {
        constexpr Tuple a{1, -2, 3, -4};
        EXPECT_EQ(a / 2, Tuple(0.5, -1, 1.5, -2));
    }

    class TupleMagnitudeTest : public ::testing::TestWithParam<std::tuple<Tuple, double> > {
    };

    TEST_P(TupleMagnitudeTest, TupleMagnitude) {
        auto [a, expected] = GetParam();
        EXPECT_EQ(a.magnitude(), expected);
    }

    INSTANTIATE_TEST_SUITE_P(
        MagnitudeSuite,
        TupleMagnitudeTest,
        ::testing::Values(
            std::make_tuple(vector(1, 0, 0), 1),
            std::make_tuple(vector(0, 1, 0), 1),
            std::make_tuple(vector(0, 0, 1), 1),
            std::make_tuple(vector(1, 2, 3), std::sqrt(14)),
            std::make_tuple(vector(-1, -2, -3), std::sqrt(14))
        )
    );

    constexpr auto epsilon = 0.00001;

    class TupleNormalizeTest : public ::testing::TestWithParam<std::tuple<Tuple, Tuple> > {
    };

    TEST_P(TupleNormalizeTest, TupleNormalize) {
        auto [a, expected] = GetParam();
        EXPECT_TRUE(approx_equals(a.normalize(), expected, epsilon));
    }

    INSTANTIATE_TEST_SUITE_P(
        MagnitudeSuite,
        TupleNormalizeTest,
        ::testing::Values(
            std::make_tuple(vector(4, 0, 0), vector(1, 0, 0)),
            std::make_tuple(vector(1, 2, 3), vector(0.26726, 0.53452, 0.80178))
        )
    );

    TEST(TupleTest, NormalizingTuple) {
        const auto v = vector(1, 20, 3);
        const auto norm = v.normalize();
        EXPECT_EQ(norm.magnitude(), 1.0);
    }

    TEST(TupleTest, DotProduct) {
        constexpr auto a = vector(1, 2, 3);
        constexpr auto b = vector(2, 3, 4);
        EXPECT_EQ(a.dot(b), 20.0);
    }

    class TupleCrossProductTest : public ::testing::TestWithParam<std::tuple<Tuple, Tuple, Tuple> > {
    };

    TEST_P(TupleCrossProductTest, CrossProduct) {
        auto [a, b, expected] = GetParam();
        EXPECT_EQ(a.cross(b), expected);
    }

    INSTANTIATE_TEST_SUITE_P(
        CrossProductSuite,
        TupleCrossProductTest,
        ::testing::Values(
            std::make_tuple(vector(1, 2, 3), vector(2, 3, 4),
                vector(-1, 2, -1)),
            std::make_tuple(vector(2, 3, 4), vector(1, 2, 3),
                vector(1, -2, 1))
        )
    );

    TEST(ColorTest, CreatingColor) {
        const auto c = color(-0.5, 0.4, 1.7);
        EXPECT_EQ(c.red(), -0.5);
        EXPECT_EQ(c.green(), 0.4);
        EXPECT_EQ(c.blue(), 1.7);
    }

    TEST(ColorTest, AddingColors) {
        const auto c1 = color(0.9, 0.6, 0.75);
        const auto c2 = color(0.7, 0.1, 0.25);
        EXPECT_EQ(c1 + c2, color(1.6, 0.7, 1.0));
    }

    TEST(ColorTest, SubtractingColors) {
        const auto c1 = color(0.9, 0.6, 0.75);
        const auto c2 = color(0.7, 0.1, 0.25);
        EXPECT_TRUE(approx_equals(c1 - c2, color(0.2, 0.5, 0.5), machine_epsilon));
    }

    TEST(ColorTest, MultiplyingByScalar) {
        const auto c = color(0.2, 0.3, 0.4);
        EXPECT_EQ(c * 2, color(0.4, 0.6, 0.8));
    }

    TEST(ColorTest, MultiplyingColors) {
        const auto c1 = color(1, 0.2, 0.4);
        const auto c2 = color(0.9, 1, 0.1);
        EXPECT_TRUE(approx_equals(c1*c2, color(0.9, 0.2, 0.04), machine_epsilon));
    }

    class TupleReflectTest : public ::testing::TestWithParam<std::tuple<Vector, Vector, Vector> > {
    };

    TEST_P(TupleReflectTest, TupleReflect) {
        auto [input, normal, expected] = GetParam();

        const auto reflected = input.reflect(normal);

        EXPECT_TRUE(approx_equals(reflected, expected, 2*machine_epsilon));
    }

    INSTANTIATE_TEST_SUITE_P(
        TupleReflectSuite,
        TupleReflectTest,
        ::testing::Values(
            std::make_tuple(vector(1, -1, 0), vector(0, 1, 0), vector(1, 1, 0)),
            std::make_tuple(
                vector(0, -1, 0),
                vector(std::numbers::sqrt2 /2, std::numbers::sqrt2 / 2, 0),
                vector(1, 0, 0))
        ));
}
