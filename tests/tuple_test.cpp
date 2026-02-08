#include <gtest/gtest.h>
#include "tuple.h"

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

    TEST(TupleTest, SubtractingTuples) {
        const auto p1 = Tuple::point(3, 2, 1);
        const auto p2 = Tuple::point(5, 6, 7);
        EXPECT_EQ(p1 - p2, Tuple::vector(-2, -4, -6));
    }
}
