#include <gtest/gtest.h>
#include <Canvas.h>

namespace rt {
    TEST(CanvasTest, CreatingCanvas) {
        const auto c = Canvas{10, 20};
        EXPECT_EQ(c.width(), 10);
        EXPECT_EQ(c.height(), 20);
        for (auto i = 0; i < c.width(); ++i) {
            for (auto j = 0; j < c.height(); ++j) {
                EXPECT_EQ(c.pixel_at(i, j), color(0, 0, 0));
            }
        }
    }
}
