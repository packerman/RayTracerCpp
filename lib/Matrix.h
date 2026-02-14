#pragma once
#include <array>
#include <cstddef>
#include <ostream>
#include <stdexcept>

namespace rt {
    template<std::size_t N>
    class Matrix {
    public:
        constexpr Matrix() : data_{} {
            for (std::size_t i = 0; i < N; ++i) {
                data_[N * i + i] = 1.0;
            }
        }

        constexpr Matrix(std::initializer_list<double> data) {
            if (data.size() != N * N) {
                throw std::runtime_error("Matrix size does not match");
            }
            std::copy(data.begin(), data.end(), data_.begin());
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

    private:
        std::array<double, N * N> data_;
    };
}
