#include <gtest/gtest.h>

#include "Pattern_test.h"

#include "Shape.h"

namespace rt {
    class TestPattern : public Pattern {
    public:
        [[nodiscard]] Color at(const Point &p) const override {
            return color(p.x, p.y, p.z);
        }
    };

    std::unique_ptr<Pattern> test_pattern() {
        return std::make_unique<TestPattern>();
    }

    TEST(PatternTest, DefaultTransformation) {
        const auto pattern = test_pattern();

        EXPECT_EQ(pattern->transform(), Transformation::identity());
    }

    TEST(PatternTest, AssignTransformation) {
        auto pattern = test_pattern();

        pattern->set_transform(translation(1, 2, 3));

        ASSERT_EQ(pattern->transform(), translation(1, 2, 3));
        EXPECT_EQ(pattern->inversed_transform(), translation(-1, -2, -3));
    }

    TEST(PatternTest, PatternObjectTransformation) {
        const auto shape = sphere();
        shape->set_transform(scaling(2, 2, 2));
        const auto pattern = test_pattern();

        const auto c = pattern->at_shape(*shape, point(2, 3, 4));

        EXPECT_EQ(c, color(1, 1.5, 2));
    }

    TEST(PatternTest, PatternWithPatternTransformation) {
        const auto shape = sphere();
        const auto pattern = test_pattern();
        pattern->set_transform(scaling(2, 2, 2));

        const auto c = pattern->at_shape(*shape, point(2, 3, 4));

        EXPECT_EQ(c, color(1, 1.5, 2));
    }

    TEST(PatternTest, PatternWithObjectAndPatternTransformation) {
        const auto shape = sphere();
        shape->set_transform(scaling(2, 2, 2));
        const auto pattern = test_pattern();
        pattern->set_transform(translation(0.5, 1, 1.5));

        const auto c = pattern->at_shape(*shape, point(2.5, 3, 3.5));

        EXPECT_EQ(c, color(0.75, 0.5, 0.25));
    }

    TEST(PatternTest, CreateStripePattern) {
        const auto pattern = stripe_pattern(white, black);

        EXPECT_EQ(pattern->a, white);
        EXPECT_EQ(pattern->b, black);
    }

    class StripePatternTestWithParam : public testing::TestWithParam<std::tuple<Point, Color> > {
    };

    TEST_P(StripePatternTestWithParam, StripePattern) {
        auto [p, c] = GetParam();

        const auto pattern = stripe_pattern(white, black);

        EXPECT_EQ(pattern->at(p), c);
    }

    INSTANTIATE_TEST_SUITE_P(
        StripePatternSuite,
        StripePatternTestWithParam,
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

    class GradientPatternTestWithParam : public testing::TestWithParam<std::tuple<Point, Color> > {
    };

    TEST_P(GradientPatternTestWithParam, GradientPattern) {
        auto [p, c] = GetParam();

        const auto pattern = gradient_pattern(white, black);

        EXPECT_EQ(pattern->at(p), c);
    }

    INSTANTIATE_TEST_SUITE_P(
        GradientPatternSuite,
        GradientPatternTestWithParam,
        ::testing::Values(
            std::make_tuple(point(0, 0, 0), white),
            std::make_tuple(point(0.25, 0, 0), color(0.75, 0.75, 0.75)),
            std::make_tuple(point(0.5, 0, 0), color(0.5, 0.5, 0.5)),
            std::make_tuple(point(0.75, 0, 0), color(0.25, 0.25, 0.25))
        ));

    class RingPatternTestWithParam : public testing::TestWithParam<std::tuple<Point, Color> > {
    };

    TEST_P(RingPatternTestWithParam, RingPattern) {
        auto [p, c] = GetParam();

        const auto pattern = ring_pattern(white, black);

        EXPECT_EQ(pattern->at(p), c);
    }

    INSTANTIATE_TEST_SUITE_P(
        RingPatternSuite,
        RingPatternTestWithParam,
        ::testing::Values(
            std::make_tuple(point(0, 0, 0), white),
            std::make_tuple(point(1, 0, 0), black),
            std::make_tuple(point(0, 0, 1), black),
            std::make_tuple(point(0.708, 0, 0.708), black)
        ));

    class CheckersPatternTestWithParam : public testing::TestWithParam<std::tuple<Point, Color> > {
    };

    TEST_P(CheckersPatternTestWithParam, RingPattern) {
        auto [p, c] = GetParam();

        const auto pattern = checkers_pattern(white, black);

        EXPECT_EQ(pattern->at(p), c);
    }

    INSTANTIATE_TEST_SUITE_P(
        CheckersPatternSuite,
        CheckersPatternTestWithParam,
        ::testing::Values(
            std::make_tuple(point(0, 0, 0), white),
            std::make_tuple(point(0.99, 0, 0), white),
            std::make_tuple(point(1.01, 0, 0), black),

            std::make_tuple(point(0, 0, 0), white),
            std::make_tuple(point(0, 0.99, 0), white),
            std::make_tuple(point(0, 1.01, 0), black),

            std::make_tuple(point(0, 0, 0), white),
            std::make_tuple(point(0, 0, 0.99), white),
            std::make_tuple(point(0, 0, 1.01), black)
        ));
}
