#include <gtest/gtest.h>

#include <Pattern.h>

namespace rt {
    TEST(PatternTest, CreateStripePattern) {
        const auto pattern = stripe_pattern(white, black);

        EXPECT_EQ(pattern->a, white);
        EXPECT_EQ(pattern->b, black);
    }

    class PatternTestWithParam : public ::testing::TestWithParam<std::tuple<Point, Color> > {
    };

    TEST_P(PatternTestWithParam, StripePattern) {
        auto [p, c] = GetParam();

        const auto pattern = stripe_pattern(white, black);

        EXPECT_EQ(pattern->color_at(p), c);
    }

    INSTANTIATE_TEST_SUITE_P(
        StripePatternSuite,
        PatternTestWithParam,
        ::testing::Values(
            std::make_tuple(point(0, 0, 0), white),
            std::make_tuple(point(0, 1, 0), white),
            std::make_tuple(point(0, 2, 0), white),

            std::make_tuple(point(0, 0, 0), white),
            std::make_tuple(point(0, 0, 1), white),
            std::make_tuple(point(0, 0, 2), white),

            std::make_tuple(point(0, 0, 0), white),
            std::make_tuple(point(0.9, 0, 0), white),
            std::make_tuple(point(1, 0, 0), black),
            std::make_tuple(point(-0.1, 0, 0), black),
            std::make_tuple(point(-1, 0, 0), black),
            std::make_tuple(point(-1.1, 0, 0), white)
        ));
}
