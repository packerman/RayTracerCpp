#include <gtest/gtest.h>

#include <Pattern.h>

#include "Shape.h"

namespace rt {
    class TestPattern : public Pattern {
    public:
        [[nodiscard]] Color at(const Point &p) const override {
            return color(p.x, p.y, p.z);
        }
    };

    std::unique_ptr<TestPattern> test_pattern() {
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

    class PatternTestWithParam : public ::testing::TestWithParam<std::tuple<Point, Color> > {
    };

    TEST_P(PatternTestWithParam, StripePattern) {
        auto [p, c] = GetParam();

        const auto pattern = stripe_pattern(white, black);

        EXPECT_EQ(pattern->at(p), c);
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
