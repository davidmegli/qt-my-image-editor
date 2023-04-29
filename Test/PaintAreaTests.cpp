#include "../paintarea.h"
#include "../myimageeditor.h"
#include <gtest/gtest.h>
#include <QColor>
#include <memory>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}


TEST(TestPaintArea, testSetPenColor) {
    /*MyImageEditor w;
    std::shared_ptr<PaintArea> p = std::make_shared<PaintArea>(&w);
    p->setPenColor(Qt::black);
    EXPECT_EQ(p->penColor(),Qt::black);*/
}
