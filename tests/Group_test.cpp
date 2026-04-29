#include <gtest/gtest.h>

#include <Group.h>

#include "Common.h"
#include "Shape_test.h"

using namespace std;

namespace rt {
    TEST(GroupTest, Creating_a_new_group) {
        const auto g = group();

        EXPECT_EQ(g->transform(), Transformation::identity());
        EXPECT_TRUE(g->empty());
    }

    TEST(GroupTest, Adding_a_child_to_a_group) {
        const auto g = group();
        auto s = test_shape();
        const auto p = test_shape();
        const auto s_ptr = s.get();

        g->add_child(std::move(s));

        EXPECT_FALSE(g->empty());
        EXPECT_TRUE(g->includes(s_ptr));
        EXPECT_FALSE(g->includes(p.get()));
        EXPECT_EQ(s_ptr->parent(), g.get());
    }

    TEST(GroupTest, Intersecting_a_ray_with_an_empty_group) {
        const auto g = group();
        const auto r = ray(point(0, 0, 0), vector(0, 0, 1));

        const auto xs = g->local_intersect(r);

        EXPECT_TRUE(xs.empty());
    }

    TEST(GroupTest, Intersecting_a_ray_with_a_nonempty_group) {
        const auto g = group();
        auto s1 = sphere();
        const auto s1_ptr = s1.get();
        auto s2 = sphere();
        const auto s2_ptr = s2.get();
        s2->set_transform(translation(0, 0, -3));
        auto s3 = sphere();
        s3->set_transform(translation(5, 0, 0));
        g->add_child(std::move(s1));
        g->add_child(std::move(s2));
        g->add_child(std::move(s3));
        const auto r = ray(point(0, 0, -5), vector(0, 0, 1));

        const auto xs = g->local_intersect(r);

        ASSERT_EQ(xs.size(), 4);
        EXPECT_EQ(xs[0].object(), s2_ptr);
        EXPECT_EQ(xs[1].object(), s2_ptr);
        EXPECT_EQ(xs[2].object(), s1_ptr);
        EXPECT_EQ(xs[3].object(), s1_ptr);
    }

    TEST(GroupTest, Intersecting_a_transformed_group) {
        const auto g = group();
        g->set_transform(scaling(2, 2, 2));
        auto s = sphere();
        s->set_transform(translation(5, 0, 0));
        g->add_child(std::move(s));

        const auto r = ray(point(10, 0, -10), vector(0, 0, 1));
        const auto xs = g->intersect(r);

        ASSERT_EQ(xs.size(), 2);
    }

    TEST(GroupTest, BoundingBoxForGroup) {
        const auto g = group();
        auto s1 = sphere();
        s1->set_transform(scaling(1, 2, 4));
        g->add_child(std::move(s1));
        auto s2 = sphere();
        s2->set_transform(translation(-10, 10, -10));
        g->add_child(std::move(s2));

        const auto b = g->bounds();

        EXPECT_EQ(b, Bounds(
                      point(-11, -2, -11),
                      point(1, 11, 4)));
    }
}
