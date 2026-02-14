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

        EXPECT_EQ(a * b, Matrix<4>({
                      20, 22, 50, 48,
                      44, 54, 114, 108,
                      40, 58, 110, 102,
                      16, 26, 46, 42
                      }));
    }

    TEST(MatrixTest, MultiplyByTuple) {
        const Matrix<4> a = {
            1, 2, 3, 4,
            2, 4, 4, 2,
            8, 6, 4, 1,
            0, 0, 0, 1
        };
        constexpr Tuple t{1, 2, 3, 1};

        EXPECT_EQ(a * t, Tuple(18, 24, 33, 1));
    }

    TEST(MatrixTest, MultiplyByIdentity) {
        const Matrix<4> a = {
            0, 1, 2, 4,
            1, 2, 4, 8,
            2, 4, 8, 16,
            4, 8, 16, 32
        };

        EXPECT_EQ(a * Matrix<4>::identity(), a);
    }

    TEST(MatrixTest, MultiplyTupleByIdentity) {
        constexpr Tuple a{1, 2, 3, 4};

        EXPECT_EQ(Matrix<4>::identity() * a, a);
    }

    TEST(MatrixTest, Transpose) {
        constexpr Matrix<4> a{
            0, 9, 3, 0,
            9, 8, 0, 8,
            1, 8, 5, 3,
            0, 0, 5, 8
        };

        EXPECT_EQ(a.transpose(), Matrix<4>({
                      0, 9, 1, 0,
                      9, 8, 8, 0,
                      3, 0, 5, 5,
                      0, 8, 3, 8
                      }));
    }

    TEST(MatrixTest, TransposeIdentity) {
        EXPECT_EQ(Matrix<4>::identity().transpose(), Matrix<4>::identity());
    }

    TEST(MatrixTest, Matrix2x2Determinant) {
        constexpr Matrix<2> a = {
            1, 5,
            -3, 2
        };

        EXPECT_EQ(a.determinant(), 17);
    }

    TEST(MatrixTest, Matrix3x3Submatrix) {
        constexpr Matrix<3> a{
            1, 5, 0,
            -3, 2, 7,
            0, 6, -3
        };

        EXPECT_EQ(a.submatrix(0, 2), Matrix<2>({
                      -3, 2,
                      0, 6
                      }));
    }

    TEST(MatrixTest, Matrix4x4Submatrix) {
        constexpr Matrix<4> a{
            -6, 1, 1, 6,
            -8, 5, 8, 6,
            -1, 0, 8, 2,
            -7, 1, -1, 1
        };

        EXPECT_EQ(a.submatrix(2, 1), Matrix<3>({
                      -6, 1, 6,
                      -8, 8, 6,
                      -7, -1, 1
                      }));
    }

    TEST(MatrixTest, Matrix3x3Minor) {
        constexpr Matrix<3> a{
            3, 5, 0,
            2, -1, -7,
            6, -1, 5
        };
        constexpr auto b = a.submatrix(1, 0);

        EXPECT_EQ(b.determinant(), 25);
        EXPECT_EQ(a.minor(1, 0), 25);
    }

    TEST(MatrixTest, Matrix3x3Cofactor) {
        constexpr Matrix<3> a{
            3, 5, 0,
            2, -1, -7,
            6, -1, 5
        };

        EXPECT_EQ(a.minor(0, 0), -12);
        EXPECT_EQ(a.cofactor(0, 0), -12);
        EXPECT_EQ(a.minor(1, 0), 25);
        EXPECT_EQ(a.cofactor(1, 0), -25);
    }

    TEST(MatrixTest, Matrix3x3Determinant) {
        constexpr Matrix<3> a{
            1, 2, 6,
            -5, 8, -4,
            2, 6, 4
        };

        EXPECT_EQ(a.cofactor(0, 0), 56);
        EXPECT_EQ(a.cofactor(0, 1), 12);
        EXPECT_EQ(a.cofactor(0, 2), -46);
        EXPECT_EQ(a.determinant(), -196);
    }

    TEST(MatrixTest, Matrix4x4Determinant) {
        constexpr Matrix<4> a{
            -2, -8, 3, 5,
            -3, 1, 7, 3,
            1, 2, -9, 6,
            -6, 7, 7, -9
        };

        EXPECT_EQ(a.cofactor(0, 0), 690);
        EXPECT_EQ(a.cofactor(0, 1), 447);
        EXPECT_EQ(a.cofactor(0, 2), 210);
        EXPECT_EQ(a.cofactor(0, 3), 51);
        EXPECT_EQ(a.determinant(), -4071);
    }
}
