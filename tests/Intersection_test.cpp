#include <gtest/gtest.h>
#include <algorithm>
#include <optional>
#include <tuple>
#include "Intersection.h"
#include "Sphere.h"

namespace rt {
    TEST(IntersectionTest, CreateIntersection) {
        Sphere s{};

        const Intersection i{3.5, &s};

        EXPECT_EQ(i.t(), 3.5);
        EXPECT_EQ(i.object(), &s);
    }

    TEST(IntersectionTest, AggregateIntersections) {
        Sphere s{};
        Intersection i1{1, &s};
        Intersection i2{2, &s};

        const auto xs = intersections({i1, i2});

        EXPECT_EQ(xs.size(), 2);
        EXPECT_EQ(xs[0].t(), 1);
        EXPECT_EQ(xs[1].t(), 2);
    }

    class HitTest : public ::testing::TestWithParam<std::tuple<std::vector<double>, std::optional<int> > > {
    protected:
        void SetUp() override {
            auto [ts, expected_index] = GetParam();
            xs.resize(ts.size());
            std::ranges::transform(ts, xs.begin(), [&](auto t) {
                return Intersection{t, &s};
            });
            expected = expected_index.transform([&](auto index) {
                return xs[index];
            });
        }

        Sphere s{};
        std::vector<Intersection> xs{};
        std::optional<Intersection> expected{};
    };

    TEST_P(HitTest, Hit) {
        const auto i = hit(xs);
        EXPECT_EQ(i, expected);
    }

    INSTANTIATE_TEST_SUITE_P(
        HitSuite,
        HitTest,
        ::testing::Values(
            std::make_tuple(std::vector<double>{1, 2}, 0),
            std::make_tuple(std::vector<double>{-1, 1}, 1),
            std::make_tuple(std::vector<double>{-2, -1}, std::optional<int>{}),
            std::make_tuple(std::vector<double>{5, 7, -3, 2}, 3)
        ));
}
