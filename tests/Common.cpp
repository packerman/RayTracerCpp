#include "Common.h"
#include <cmath>

testing::AssertionResult approx_equals(const double a, const double b, const double epsilon) {
    if (std::abs(a - b) < epsilon) {
        return testing::AssertionSuccess();
    }
    return testing::AssertionFailure() << a << " is not equal to " << b << " (with error " << epsilon << ")";
}

testing::AssertionResult approx_equals(const rt::Tuple &a, const rt::Tuple &b, const double epsilon) {
    if (auto result = approx_equals(a.x, b.x, epsilon); !result) {
        return result << " at Tuple::x";
    }
    if (auto result = approx_equals(a.y, b.y, epsilon); !result) {
        return result << " at Tuple::y";
    }
    if (auto result = approx_equals(a.z, b.z, epsilon); !result) {
        return result << " at Tuple::z";
    }
    if (auto result = approx_equals(a.w, b.w, epsilon); !result) {
        return result << " at Tuple::w";
    }
    return testing::AssertionSuccess();
}
