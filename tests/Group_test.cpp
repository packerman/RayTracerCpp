#include <gtest/gtest.h>

#include <Group.h>

namespace rt {

    TEST(GroupTest, Creating_a_new_group) {
        auto g = group();

        EXPECT_EQ(g->transform(), Transformation::identity());
        EXPECT_TRUE(g->empty());
    }
}