#include <gtest/gtest.h>
#include "World.h"

#include "Common.h"
#include "Pattern_test.h"

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

    TEST(WorldTest, ShadeHitInShadow) {
        World w{};
        w.add_light(point_light(point(0, 0, -10), color(1, 1, 1)));
        auto s1 = sphere();
        w.add_object(std::move(s1));
        auto s2 = sphere();
        s2->set_transform(translation(0, 0, 10));
        constexpr Ray r{point(0, 0, 5), vector(0, 0, 1)};
        const Intersection i{4, s2.get()};
        w.add_object(std::move(s2));

        const auto comps = prepare_computations(i, r);
        const auto c = w.shade_hit(comps);

        EXPECT_EQ(c, color(0.1, 0.1, 0.1));
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

    class ShadowTest : public ::testing::TestWithParam<std::tuple<Point, bool> > {
    };

    TEST_P(ShadowTest, IsShadowed) {
        auto [p, expected] = GetParam();

        const auto w = default_world();
        auto &light = w.light(0);

        EXPECT_EQ(w.is_shadowed(p, *light), expected);
    }

    INSTANTIATE_TEST_SUITE_P(
        ShadowSuite,
        ShadowTest,
        ::testing::Values(
            std::make_tuple(point(0, 10, 0), false),
            std::make_tuple(point(10, -10, 10), true),
            std::make_tuple(point(-20, 20, -20), false),
            std::make_tuple(point(-2, 2, -2), false)
        ));

    TEST(WorldTest, ReflectedColorForNonreflectiveMaterial) {
        const auto w = default_world();
        constexpr Ray r{point(0, 0, 0), vector(0, 0, 1)};
        auto &shape = w.object(1);
        shape->material().ambient = 1;
        const Intersection i{1, shape.get()};

        const auto comps = prepare_computations(i, r);
        const auto color = w.reflected_color(comps);

        EXPECT_EQ(color, black);
    }

    TEST(WorldTest, ReflectedColorReflectiveMaterial) {
        auto w = default_world();
        auto shape = plane();
        shape->material().reflective = 0.5;
        shape->set_transform(translation(0, -1, 0));
        const Intersection i{std::numbers::sqrt2, shape.get()};
        w.add_object(std::move(shape));
        constexpr Ray r{
            point(0, 0, -3),
            vector(0, -std::numbers::sqrt2 / 2, std::numbers::sqrt2 / 2)
        };

        const auto comps = prepare_computations(i, r);
        const auto color = w.reflected_color(comps);

        EXPECT_TRUE(approx_equals(color, rt::color(0.19032, 0.2379, 0.14274), 1e-4));
    }

    TEST(WorldTest, ShadeHitReflectiveMaterial) {
        auto w = default_world();
        auto shape = plane();
        shape->material().reflective = 0.5;
        shape->set_transform(translation(0, -1, 0));
        const Intersection i{std::numbers::sqrt2, shape.get()};
        w.add_object(std::move(shape));
        constexpr Ray r{
            point(0, 0, -3),
            vector(0, -std::numbers::sqrt2 / 2, std::numbers::sqrt2 / 2)
        };

        const auto comps = prepare_computations(i, r);
        const auto color = w.shade_hit(comps);

        EXPECT_TRUE(approx_equals(color, rt::color(0.87677, 0.92436, 0.82918), 1e-4));
    }

    TEST(WorldTest, ColorAtWithMutuallyReflectiveSurfaces) {
        World w{};
        w.add_light(point_light(point(0, 0, 0), color(1, 1, 1)));
        auto lower = plane();
        lower->material().reflective = 1;
        lower->set_transform(translation(0, -1, 0));
        w.add_object(std::move(lower));
        auto upper = plane();
        upper->material().reflective = 1;
        upper->set_transform(translation(0, 1, 0));
        w.add_object(std::move(upper));
        constexpr Ray r{point(0, 0, 0), vector(0, 1, 0)};

        // ReSharper disable once CppDFAUnreadVariable
        Color c;
        EXPECT_NO_FATAL_FAILURE({
            c = w.color_at(r);
            });
    }

    TEST(WorldTest, ReflectedColorAtTheMaximumRecursiveDepth) {
        auto w = default_world();
        auto shape = plane();
        shape->material().reflective = 0.5;
        shape->set_transform(translation(0, -1, 0));
        const Intersection i{std::numbers::sqrt2, shape.get()};
        w.add_object(std::move(shape));
        constexpr Ray r{
            point(0, 0, -3),
            vector(0, -std::numbers::sqrt2 / 2, std::numbers::sqrt2 / 2)
        };

        const auto comps = prepare_computations(i, r);
        const auto color = w.reflected_color(comps, 0);

        EXPECT_EQ(color, black);
    }

    TEST(WorldTest, The_refracted_color_with_an_opaque_surface) {
        const auto w = default_world();
        auto &shape = w.object(0);
        constexpr Ray r{point(0, 0, -5), vector(0, 0, 1)};
        Intersections xs{{4, shape.get()}, {6, shape.get()}};

        const auto comps = prepare_computations(xs.data()[0], r, xs);
        const auto c = w.refracted_color(comps, 5);

        EXPECT_EQ(c, black);
    }

    TEST(WorldTest, The_refracted_color_at_the_maximum_recursive_depth) {
        const auto w = default_world();
        auto &shape = w.object(0);
        shape->material().transparency = 1;
        shape->material().refractive_index = 1.5;
        constexpr Ray r{point(0, 0, -5), vector(0, 0, 1)};
        Intersections xs{{4, shape.get()}, {6, shape.get()}};

        const auto comps = prepare_computations(xs.data()[0], r, xs);
        const auto c = w.refracted_color(comps, 0);

        EXPECT_EQ(c, black);
    }

    TEST(WorldTest, The_refracted_color_under_total_internal_reflection) {
        const auto w = default_world();
        auto &shape = w.object(0);
        shape->material().transparency = 1;
        shape->material().refractive_index = 1.5;
        constexpr Ray r{point(0, 0, std::numbers::sqrt2 / 2), vector(0, 1, 0)};
        Intersections xs{{-std::numbers::sqrt2 / 2, shape.get()}, {std::numbers::sqrt2 / 2, shape.get()}};

        const auto comps = prepare_computations(xs.data()[1], r, xs);
        const auto c = w.refracted_color(comps, 5);

        EXPECT_EQ(c, black);
    }

    TEST(WorldTest, The_refracted_color_with_a_refracted_ray) {
        const auto w = default_world();
        auto &a = w.object(0);
        a->material().ambient = 1;
        a->material().pattern = test_pattern();
        auto &b = w.object(1);
        b->material().transparency = 1;
        b->material().refractive_index = 1.5;
        constexpr Ray r{point(0, 0, 0.1), vector(0, 1, 0)};
        Intersections xs{
            {-0.9899, a.get()},
            {-0.4899, b.get()},
            {0.4899, b.get()},
            {0.9899, a.get()}
        };

        const auto comps = prepare_computations(xs.data()[2], r, xs);
        const auto c = w.refracted_color(comps, 5);

        EXPECT_TRUE(approx_equals(c, color(0, 0.99888, 0.04725), 1e-4));
    }

    TEST(WorldTest, Shade_hit_with_a_transparent_material) {
        auto w = default_world();
        auto floor = plane();
        floor->set_transform(translation(0, -1, 0));
        floor->material().transparency = 0.5;
        floor->material().refractive_index = 1.5;
        auto ball = sphere();
        ball->material().color = color(1, 0, 0);
        ball->material().ambient = 0.5;
        ball->set_transform(translation(0, -3.5, -0.5));
        w.add_object(std::move(ball));
        constexpr Ray r{point(0, 0, -3), vector(0, -std::numbers::sqrt2 / 2, std::numbers::sqrt2 / 2)};
        Intersections xs{{std::numbers::sqrt2, floor.get()}};
        w.add_object(std::move(floor));

        const auto comps = prepare_computations(xs.data()[0], r, xs);
        const auto color = w.shade_hit(comps, 5);

        EXPECT_TRUE(approx_equals(color, rt::color(0.93642, 0.68642, 0.68642), 1e-5));
    }

    TEST(WorldTest, Shade_hit_with_a_reflective_transparent_material) {
        auto w = default_world();
        auto floor = plane();
        floor->set_transform(translation(0, -1, 0));
        floor->material().reflective = 0.5;
        floor->material().transparency = 0.5;
        floor->material().refractive_index = 1.5;
        auto ball = sphere();
        ball->material().color = color(1, 0, 0);
        ball->material().ambient = 0.5;
        ball->set_transform(translation(0, -3.5, -0.5));
        w.add_object(std::move(ball));
        constexpr Ray r{point(0, 0, -3), vector(0, -std::numbers::sqrt2 / 2, std::numbers::sqrt2 / 2)};
        Intersections xs{{std::numbers::sqrt2, floor.get()}};
        w.add_object(std::move(floor));

        const auto comps = prepare_computations(xs.data()[0], r, xs);
        const auto color = w.shade_hit(comps, 5);

        EXPECT_TRUE(approx_equals(color, rt::color(0.93391, 0.69643, 0.69243), 1e-5));
    }
}
