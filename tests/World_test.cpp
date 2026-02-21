#include <gtest/gtest.h>
#include "World.h"

namespace rt {

    TEST(WorldTest, CreateWorld) {
        const World w{};

        EXPECT_TRUE(w.objects().empty());
        EXPECT_FALSE(w.light());
    }

    TEST(WorldTest, DefaultWorld) {
        const Light light{point(-10, 10, -10), color(1, 1, 1)};

        const auto s1 = std::make_unique<Sphere>();
        s1->material().color = color(0.8, 1.0, 0.6);
        s1->material().diffuse = 0.7;
        s1->material().specular = 0.2;

        const auto s2 = std::make_unique<Sphere>();
        s2->set_transform(scaling(0.5, 0.5, 0.5));

        auto w = default_world();

        EXPECT_EQ(w.light(), light);
        EXPECT_EQ(w.object(0)->material().color, color(0.8, 1, 0.6));
        EXPECT_EQ(w.object(0)->material().diffuse, 0.7);
        EXPECT_EQ(w.object(0)->material().specular, 0.2);
        EXPECT_EQ(w.object(1)->transform(), scaling(0.5, 0.5, 0.5));
    }

    TEST(WorldTest, Intersect) {
        auto w = default_world();
        Ray r{point(0, 0, -5), vector(0, 0, 1)};

        auto xs = w.intersect(r);

        EXPECT_EQ(xs.size(), 4);
        EXPECT_EQ(xs[0].t(), 4);
        EXPECT_EQ(xs[1].t(), 4.5);
        EXPECT_EQ(xs[2].t(), 5.5);
        EXPECT_EQ(xs[3].t(), 6);
    }
}