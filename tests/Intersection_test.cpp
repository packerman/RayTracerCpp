#include <gtest/gtest.h>
#include <algorithm>
#include <optional>
#include <tuple>
#include "Intersection.h"

#include "Common.h"
#include "Shape.h"

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

        const auto xs = Intersections({i1, i2}).data();

        EXPECT_EQ(xs.size(), 2);
        EXPECT_EQ(xs[0].t(), 1);
        EXPECT_EQ(xs[1].t(), 2);
    }

    Intersections make_intersections(Sphere *object, const std::vector<double> &ts) {
        std::vector<Intersection> xs(ts.size());
        std::ranges::transform(ts, xs.begin(), [&](auto t) {
            return Intersection{t, object};
        });
        return Intersections{xs};
    }

    class HitTest : public ::testing::TestWithParam<std::tuple<std::vector<double>, std::optional<double> > > {
    protected:
        void SetUp() override {
            auto [ts, expected_t] = GetParam();
            object = std::make_unique<Sphere>();
            xs = make_intersections(object.get(), ts);
            if (expected_t) {
                expected = Intersection{*expected_t, object.get()};
            } else {
                expected = {};
            }
        }

        std::unique_ptr<Sphere> object{};
        Intersections xs{};
        std::optional<Intersection> expected{};
    };

    TEST_P(HitTest, Hit) {
        const auto i = xs.hit();
        EXPECT_EQ(i, expected);
    }

    INSTANTIATE_TEST_SUITE_P(
        HitSuite,
        HitTest,
        ::testing::Values(
            std::make_tuple(std::vector<double>{1, 2}, 1),
            std::make_tuple(std::vector<double>{-1, 1}, 1),
            std::make_tuple(std::vector<double>{-2, -1}, std::optional<int>{}),
            std::make_tuple(std::vector<double>{5, 7, -3, 2}, 2)
        ));

    TEST(IntersectionTest, Precompute) {
        constexpr Ray r{point(0, 0, -5), vector(0, 0, 1)};
        Sphere shape{};
        const Intersection i{4, &shape};

        const auto comps = prepare_computations(i, r);

        EXPECT_EQ(comps.t, i.t());
        EXPECT_EQ(comps.object, i.object());
        EXPECT_EQ(comps.point, point(0, 0, -1));
        EXPECT_EQ(comps.eye_v, vector(0, 0, -1));
        EXPECT_EQ(comps.normal_v, vector(0, 0, -1));
    }

    TEST(IntersectionTest, HitOutside) {
        constexpr Ray r{point(0, 0, -5), vector(0, 0, 1)};
        Sphere shape{};
        const Intersection i{4, &shape};

        const auto comps = prepare_computations(i, r);

        EXPECT_EQ(comps.inside, false);
    }

    TEST(IntersectionTest, HitInside) {
        constexpr Ray r{point(0, 0, 0), vector(0, 0, 1)};
        Sphere shape{};
        const Intersection i{1, &shape};

        const auto comps = prepare_computations(i, r);

        EXPECT_EQ(comps.point, point(0, 0, 1));
        EXPECT_EQ(comps.eye_v, vector(0, 0, -1));
        EXPECT_EQ(comps.inside, true);
        EXPECT_EQ(comps.normal_v, vector(0, 0, -1));
    }

    TEST(IntersectionTest, OffsetThePoint) {
        constexpr Ray r{point(0, 0, -5), vector(0, 0, 1)};
        const auto shape = sphere();
        shape->set_transform(translation(0, 0, 1));
        const Intersection i{5, shape.get()};

        const auto comps = prepare_computations(i, r);

        EXPECT_LT(comps.over_point.z, -shadow_epsilon/2);
        EXPECT_GT(comps.point.z, comps.over_point.z);
    }
}
