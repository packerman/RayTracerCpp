#include <gtest/gtest.h>

#include <Group.h>
#include "Shape_test.h"

namespace rt {
    TEST(GroupTest, Creating_a_new_group) {
        auto g = group();

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
        auto g = group();
        const auto r = ray(point(0, 0, 0), vector(0, 0, 1));

        const auto xs = g->local_intersect(r);

        EXPECT_TRUE(xs.empty());
    }
}
