#include <gtest/gtest.h>
#include <Bounds.h>

#include "Common.h"

using namespace std;

namespace rt {
    class BoundsTransformTest : public testing::TestWithParam<std::tuple<Transformation, Bounds> > {
    };

    TEST_P(BoundsTransformTest, Transform_bounds) {
        auto [transform, expected] = GetParam();

        constexpr Bounds bounds{point(-1, -1, -1), point(1, 1, 1)};

        const auto [minimum, maximum] = bounds.transform(transform);

        EXPECT_TRUE(approx_equals(minimum, expected.minimum, 1e-10));
        EXPECT_TRUE(approx_equals(maximum, expected.maximum, 1e-10));
    }

    INSTANTIATE_TEST_SUITE_P(
        BoundsTransformTestSuite,
        BoundsTransformTest,
        testing::Values(
            make_tuple(scaling(1, 2, 3), Bounds{
                point(-1, -2, -3),
                point(1, 2, 3)
                }),
            make_tuple(rotation_x(numbers::pi/2), Bounds{
                point(-1, -1, -1),
                point(1, 1, 1)
                }),
            make_tuple(rotation_y(numbers::pi/2), Bounds{
                point(-1, -1, -1),
                point(1, 1, 1)
                }),
            make_tuple(rotation_z(numbers::pi/2), Bounds{
                point(-1, -1, -1),
                point(1, 1, 1)
                }),
            make_tuple(rotation_z(numbers::pi/2)*rotation_y(numbers::pi/2), Bounds{
                point(-1, -1, -1),
                point(1, 1, 1)
                }),
            make_tuple(rotation_x(numbers::pi/4), Bounds{
                point(-1, -numbers::sqrt2, -numbers::sqrt2),
                point(1, numbers::sqrt2, numbers::sqrt2)
                }),
            make_tuple(rotation_y(numbers::pi/4), Bounds{
                point(-numbers::sqrt2, -1, -numbers::sqrt2),
                point(numbers::sqrt2, 1, numbers::sqrt2)
                }),
            make_tuple(rotation_z(numbers::pi/4), Bounds{
                point(-numbers::sqrt2, -numbers::sqrt2, -1),
                point(numbers::sqrt2, numbers::sqrt2, 1)
                }),
            make_tuple(rotation_z(numbers::pi/4)*rotation_y(numbers::pi/4), Bounds{
                point(-1/numbers::sqrt2 - 1, -1/numbers::sqrt2 - 1, -numbers::sqrt2),
                point(1/numbers::sqrt2 + 1, 1/numbers::sqrt2 + 1, numbers::sqrt2)
                }),
            make_tuple(rotation_x(numbers::pi/6), Bounds{
                point(-1, -numbers::sqrt3/2 - 0.5, -numbers::sqrt3/2 - 0.5),
                point(1, numbers::sqrt3/2 + 0.5, numbers::sqrt3/2 + 0.5)
                }),
            make_tuple(rotation_y(numbers::pi/6), Bounds{
                point(-numbers::sqrt3/2 - 0.5, -1, -numbers::sqrt3/2 - 0.5),
                point(numbers::sqrt3/2 + 0.5, 1, numbers::sqrt3/2 + 0.5)
                }),
            make_tuple(rotation_z(numbers::pi/6), Bounds{
                point(-numbers::sqrt3/2 - 0.5, -numbers::sqrt3/2 - 0.5, -1),
                point(numbers::sqrt3/2 + 0.5, numbers::sqrt3/2 + 0.5, 1)
                }),
            make_tuple(rotation_z(numbers::pi/6)*rotation_y(numbers::pi/6), Bounds{
                point(-1.25-numbers::sqrt3/4, -3*numbers::sqrt3/4-0.25, -numbers::sqrt3/2 - 0.5),
                point(1.25+numbers::sqrt3/4, 3*numbers::sqrt3/4+0.25, numbers::sqrt3/2 + 0.5)
                }),
            make_tuple(translation(1, 1, 1), Bounds{
                point(0, 0, 0),
                point(2, 2, 2)
                })
        )
    );

    TEST(BoundsTest, Combine_bounds) {
        const std::vector<Bounds> boxes{
            {point(-1, -1, -1), point(1, 1, 1)},
            {point(-1, 10, -3), point(1, 11, 14)},
            {point(-1, -10, 3), point(1, -8, 12)}
        };

        const auto result = combine_bounds(boxes);

        EXPECT_EQ(result, Bounds(
                      point(-1, -10, -3),
                      point(1, 11, 14)));
    }
}
