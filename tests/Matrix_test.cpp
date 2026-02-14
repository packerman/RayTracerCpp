#include <gtest/gtest.h>
#include <Matrix.h>

namespace rt {
    TEST(MatrixTest, CreateMatrix4x4) {
        Matrix<4> m = {
            1, 2, 3, 4,
            5.5, 6.5, 7.5, 8.5,
            9, 10, 11, 12,
            13.5, 14.5, 15.5, 16.5
        };

        EXPECT_EQ(m(0, 0), 1);
        EXPECT_EQ(m(0, 3), 4);
        EXPECT_EQ(m(1, 0), 5.5);
        EXPECT_EQ(m(1, 2), 7.5);
        EXPECT_EQ(m(2, 2), 11);
        EXPECT_EQ(m(3, 0), 13.5);
        EXPECT_EQ(m(3, 2), 15.5);
    }

    TEST(MatrixTest, CreateMatrix2x2) {
        Matrix<2> m = {
            -3, 5,
            1, -2
        };

        EXPECT_EQ(m(0, 0), -3);
        EXPECT_EQ(m(0, 1), 5);
        EXPECT_EQ(m(1, 0), 1);
        EXPECT_EQ(m(1, 1), -2);
    }

    TEST(MatrixTest, CreateMatrix3x3) {
        Matrix<3> m = {
            -3, 5, 0,
            1, -2, -7,
            0, 1, 1
        };

        EXPECT_EQ(m(0, 0), -3);
        EXPECT_EQ(m(1, 1), -2);
        EXPECT_EQ(m(2, 2), 1);
    }

    TEST(MatrixTest, MatrixEquality) {
        const Matrix<4> a = {
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 8, 7, 6,
            5, 4, 3, 2
        };
        const Matrix<4> b = {
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 8, 7, 6,
            5, 4, 3, 2
        };

        EXPECT_EQ(a, b);
    }

    TEST(MatrixTest, MatrixInequality) {
        const Matrix<4> a = {
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 8, 7, 6,
            5, 4, 3, 2
        };
        const Matrix<4> b = {
            2, 3, 4, 5,
            6, 7, 8, 9,
            8, 7, 6, 5,
            4, 3, 2, 1
        };

        EXPECT_NE(a, b);
        EXPECT_TRUE(a != b);
    }

    TEST(MatrixTest, MultiplyMatrix) {
        const Matrix<4> a = {
            1, 2, 3, 4,
            5, 6, 7, 8,
            9, 8, 7, 6,
            5, 4, 3, 2
        };
        constexpr Matrix<4> b = {
            -2, 1, 2, 3,
            3, 2, 1, -1,
            4, 3, 6, 5,
            1, 2, 7, 8
        };

        const Matrix<4> c = {
            20, 22, 50, 48,
            44, 54, 114, 108,
            40, 58, 110, 102,
            16, 26, 46, 42
        };
        EXPECT_EQ(a * b, c);
    }

    TEST(MatrixTest, MultiplyByTuple) {
        const Matrix<4> a = {
            1, 2, 3, 4,
            2, 4, 4, 2,
            8, 6, 4, 1,
            0, 0, 0, 1
        };
        const Tuple t{1, 2, 3, 1};

        const Tuple expected{18, 24, 33, 1};
        EXPECT_EQ(a * t, expected);
    }
}
