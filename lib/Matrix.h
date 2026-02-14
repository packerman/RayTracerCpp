#pragma once
#include <array>
#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <initializer_list>

#include "Tuple.h"

namespace rt {
    template<std::size_t N>
    using Storage = std::array<double, N * N>;

    template<std::size_t N>
    class Matrix {
    public:
        constexpr Matrix() : data_{} {
        }

        constexpr Matrix(std::initializer_list<double> data) {
            if (data.size() != N * N) {
                throw std::runtime_error("Matrix size does not match");
            }
            std::copy(data.begin(), data.end(), data_.begin());
        }

        explicit constexpr Matrix(const Storage<N> &data) : data_(data) {
        }

        constexpr double &operator()(const std::size_t i, const std::size_t j) {
            return data_[i * N + j];
        }

        constexpr double operator()(const std::size_t i, const std::size_t j) const {
            return data_[i * N + j];
        }

        friend bool operator==(const Matrix &a, const Matrix &b) {
            return a.data_ == b.data_;
        }

        friend bool operator!=(const Matrix &a, const Matrix &b) {
            return !(a == b);
        }

        friend constexpr Matrix operator*(const Matrix &a, const Matrix &b) {
            Matrix m;
            for (std::size_t i = 0; i < N; ++i) {
                for (std::size_t j = 0; j < N; ++j) {
                    double sum = 0;
                    for (std::size_t k = 0; k < N; ++k) {
                        sum += a(i, k) * b(k, j);
                    }
                    m(i, j) = sum;
                }
            }
            return m;
        }

        [[nodiscard]] constexpr Matrix transpose() const {
            Matrix m;
            for (std::size_t i = 0; i < N; ++i) {
                for (std::size_t j = 0; j < N; ++j) {
                    m(i, j) = data_[j * N + i];
                }
            }
            return m;
        }

        [[nodiscard]] constexpr double determinant() const {
            static_assert(N >= 2);
            if (N == 2) {
                return data_[0] * data_[3] - data_[1] * data_[2];
            }
            return 0;
        }

        [[nodiscard]] constexpr Matrix<N - 1> submatrix(const std::size_t k, const std::size_t l) const {
            Storage<N - 1> data;
            std::size_t p = 0;
            for (std::size_t i = 0; i < N; ++i) {
                for (std::size_t j = 0; j < N; ++j) {
                    if (i != k && j != l) {
                        data[p++] = data_[i * N + j];
                    }
                }
            }
            return Matrix<N - 1>(data);
        }

        friend std::ostream &operator<<(std::ostream &os, const Matrix &obj) {
            os << "|";
            for (std::size_t i = 0; i < N; ++i) {
                for (std::size_t j = 0; j < N; ++j) {
                    os << " " << obj.data_[i * N + j];
                }
                os << " |";
            }
            return os;
        }

        constexpr static Matrix identity() {
            Matrix m{};
            for (std::size_t i = 0; i < N; ++i) {
                m(i, i) = 1;
            }
            return m;
        }

    private:
        Storage<N> data_;
    };

    constexpr Tuple operator*(const Matrix<4> &a, const Tuple &t) {
        return {
            a(0, 0) * t.x + a(0, 1) * t.y + a(0, 2) * t.z + a(0, 3) * t.w,
            a(1, 0) * t.x + a(1, 1) * t.y + a(1, 2) * t.z + a(1, 3) * t.w,
            a(2, 0) * t.x + a(2, 1) * t.y + a(2, 2) * t.z + a(2, 3) * t.w,
            a(3, 0) * t.x + a(3, 1) * t.y + a(3, 2) * t.z + a(3, 3) * t.w,
        };
    }
}
