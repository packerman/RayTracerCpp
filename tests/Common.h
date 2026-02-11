#pragma once

#include <gtest/gtest.h>
#include "Tuple.h"
#include <limits>

constexpr auto machine_epsilon = std::numeric_limits<double>::epsilon();

testing::AssertionResult approx_equals(double a, double b, double epsilon);

testing::AssertionResult approx_equals(const rt::Tuple& a, const rt::Tuple& b, double epsilon);
