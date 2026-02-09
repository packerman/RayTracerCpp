#include <gtest/gtest.h>
#include <tuple>
#include "Tuple.h"

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
        const auto p = Tuple::point(4, -4, 3);
        EXPECT_EQ(p, Tuple(4, -4, 3, 1));
    }

    TEST(TupleTest, VectorMethod) {
        const auto p = Tuple::vector(4, -4, 3);
        EXPECT_EQ(p, Tuple(4, -4, 3, 0));
    }

    TEST(TupleTest, AddingTuples) {
        constexpr Tuple a1{3, -2, 5, 1};
        constexpr Tuple a2{-2, 3, 1, 0};
        EXPECT_EQ(a1 + a2, Tuple(1, 1, 6, 1));
    }

    class TupleSubtractTest : public ::testing::TestWithParam<std::tuple<Tuple, Tuple, Tuple>> {
    };

    TEST_P(TupleSubtractTest, SubtractingTuples) {
        auto [a, b, expected] = GetParam();
        EXPECT_EQ(a - b, expected);
    }

    INSTANTIATE_TEST_SUITE_P(
        SubtractSuite,
        TupleSubtractTest,
        ::testing::Values(
            std::make_tuple(Tuple::point(3, 2, 1), Tuple::point(5, 6, 7),
                Tuple::vector(-2, -4, -6)),
            std::make_tuple(Tuple::point(3, 2, 1), Tuple::vector(5, 6, 7),
                Tuple::point(-2, -4, -6)),
            std::make_tuple(Tuple::vector(3, 2, 1), Tuple::vector(5, 6, 7),
                Tuple::vector(-2, -4, -6)),
            std::make_tuple(Tuple::vector(0, 0, 0), Tuple::vector(1, -2, 3),
                Tuple::vector(-1, 2, -3))
        )
    );

    TEST(TupleTest, NegatingTuple) {
        const Tuple a{1, -2, 3, -4};
        EXPECT_EQ(-a, Tuple(-1, 2, -3, 4));
    }

    class TupleMultiplyTest : public ::testing::TestWithParam<std::tuple<Tuple, double, Tuple>> {
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
}
