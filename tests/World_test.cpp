#include <gtest/gtest.h>
#include "World.h"

#include "Common.h"

namespace rt {
    TEST(WorldTest, CreateWorld) {
        const World w{};

        EXPECT_TRUE(w.objects().empty());
        EXPECT_TRUE(w.lights().empty());
    }

    TEST(WorldTest, DefaultWorld) {
        const auto w = default_world();

        EXPECT_EQ(w.light(0)->position, point(-10, 10, -10));
        EXPECT_EQ(w.light(0)->intensity, color(1, 1, 1));
        EXPECT_EQ(w.object(0)->material().color, color(0.8, 1, 0.6));
        EXPECT_EQ(w.object(0)->material().diffuse, 0.7);
        EXPECT_EQ(w.object(0)->material().specular, 0.2);
        EXPECT_EQ(w.object(1)->transform(), scaling(0.5, 0.5, 0.5));
    }

    TEST(WorldTest, Intersect) {
        const auto w = default_world();
        constexpr Ray r{point(0, 0, -5), vector(0, 0, 1)};

        const auto xs = w.intersect(r).data();

        EXPECT_EQ(xs.size(), 4);
        EXPECT_EQ(xs[0].t(), 4);
        EXPECT_EQ(xs[1].t(), 4.5);
        EXPECT_EQ(xs[2].t(), 5.5);
        EXPECT_EQ(xs[3].t(), 6);
    }

    TEST(WorldTest, ShadeIntersection) {
        const auto w = default_world();
        constexpr Ray r{point(0, 0, -5), vector(0, 0, 1)};
        auto &shape = w.object(0);
        const Intersection i{4, shape.get()};

        const auto comps = prepare_computations(i, r);
        const auto c = w.shade_hit(comps);

        EXPECT_TRUE(approx_equals(c, color(0.38066, 0.47583, 0.2855), 1e-5));
    }

    TEST(WorldTest, ShadeIntersectionFromTheInside) {
        auto w = default_world();
        w.remove_light(0);
        w.add_light(std::make_unique<Light>(point(0, 0.25, 0), color(1, 1, 1)));
        constexpr Ray r{point(0, 0, 0), vector(0, 0, 1)};
        auto &shape = w.object(1);
        const Intersection i{0.5, shape.get()};

        const auto comps = prepare_computations(i, r);
        const auto c = w.shade_hit(comps);

        EXPECT_TRUE(approx_equals(c, color(0.90498, 0.90498, 0.90498), 1e-5));
    }

    TEST(WorldTest, ColorWhenRayMisses) {
        const auto w = default_world();
        constexpr Ray r{point(0, 0, -5), vector(0, 1, 0)};

        const auto c = w.color_at(r);

        EXPECT_EQ(c, color(0, 0, 0));
    }

    TEST(WorldTest, ColorWhenRayHits) {
        const auto w = default_world();
        constexpr Ray r{point(0, 0, -5), vector(0, 0, 1)};

        const auto c = w.color_at(r);

        EXPECT_TRUE(approx_equals(c, color(0.38066, 0.47583, 0.2855), 1e-5));
    }

    TEST(WorldTest, ColorWithIntersectionBehindTheRay) {
        const auto w = default_world();
        auto &outer = w.object(0);
        outer->material().ambient = 1;
        auto &inner = w.object(1);
        inner->material().ambient = 1;
        constexpr Ray r{point(0, 0, 0.75), vector(0, 0, -1)};

        const auto c = w.color_at(r);

        EXPECT_EQ(c, inner->material().color);
    }
}
