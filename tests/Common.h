#pragma once

#include <gtest/gtest.h>
#include "Tuple.h"
#include <limits>
#include <vector>
#include <string>

#include "Matrix.h"

constexpr auto machine_epsilon = std::numeric_limits<double>::epsilon();

testing::AssertionResult approx_equals(double a, double b, double epsilon);

testing::AssertionResult approx_equals(const rt::Tuple& a, const rt::Tuple& b, double epsilon);

template <std::size_t N>
testing::AssertionResult approx_equals(const rt::Matrix<N> &a, const rt::Matrix<N> &b, const double epsilon) {
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            if (auto result = approx_equals(a(i, j), b(i, j), epsilon); !result) {
                return result << " at matrix(" << i << ", " << j << ")";
            }
        }
    }
    return testing::AssertionSuccess();
}

std::vector<std::string> string_lines(const std::string& s);
