#include "gtest/gtest.h"
#include "gmock/gmock.h" //EXPECT_THAT macro, matchers

#include "Color.hpp"


TEST(ColorTest, BlackOpaque)
{
    Color black(255);

    EXPECT_EQ(0,   black.r);
    EXPECT_EQ(0,   black.g);
    EXPECT_EQ(0,   black.b);
    EXPECT_EQ(255, black.a);
}

TEST(ColorTest, BlackTransparent)
{
    Color black(0);

    EXPECT_EQ(0, black.r);
    EXPECT_EQ(0, black.g);
    EXPECT_EQ(0, black.b);
    EXPECT_EQ(0, black.a);
}

TEST(ColorTest, WhiteOpaque)
{
    Color white(0xFFFF'FFFF);

    EXPECT_EQ(255, white.r);
    EXPECT_EQ(255, white.g);
    EXPECT_EQ(255, white.b);
    EXPECT_EQ(255, white.a);
}

TEST(ColorTest, WhiteTransparent)
{
    Color white(0xFFFF'FF00);

    EXPECT_EQ(255, white.r);
    EXPECT_EQ(255, white.g);
    EXPECT_EQ(255, white.b);
    EXPECT_EQ(0,   white.a);
}

TEST(ColorTest, RedOpaque)
{
    Color red(0xFF00'00FF);

    EXPECT_EQ(255, red.r);
    EXPECT_EQ(0,   red.g);
    EXPECT_EQ(0,   red.b);
    EXPECT_EQ(255, red.a);
}

TEST(ColorTest, RedTransparent)
{
    Color red(0xFF00'0000);

    EXPECT_EQ(255, red.r);
    EXPECT_EQ(0,   red.g);
    EXPECT_EQ(0,   red.b);
    EXPECT_EQ(0,   red.a);
}

TEST(ColorTest, GreenOpaque)
{
    Color green(0x00FF'00FF);

    EXPECT_EQ(0,   green.r);
    EXPECT_EQ(255, green.g);
    EXPECT_EQ(0,   green.b);
    EXPECT_EQ(255, green.a);
}

TEST(ColorTest, GreenTransparent)
{
    Color green(0x00FF'0000);

    EXPECT_EQ(0,   green.r);
    EXPECT_EQ(255, green.g);
    EXPECT_EQ(0,   green.b);
    EXPECT_EQ(0,   green.a);
}

TEST(ColorTest, BlueOpaque)
{
    Color blue(0x00'FFFF);

    EXPECT_EQ(0,   blue.r);
    EXPECT_EQ(0,   blue.g);
    EXPECT_EQ(255, blue.b);
    EXPECT_EQ(255, blue.a);
}

TEST(ColorTest, BlueTransparent)
{
    Color blue(0x00'FF00);

    EXPECT_EQ(0,   blue.r);
    EXPECT_EQ(0,   blue.g);
    EXPECT_EQ(255, blue.b);
    EXPECT_EQ(0,   blue.a);
}

TEST(ColorTest, GrayHalfTransparent)
{
    Color gray(0x8080'8080);

    EXPECT_EQ(128, gray.r);
    EXPECT_EQ(128, gray.g);
    EXPECT_EQ(128, gray.b);
    EXPECT_EQ(128, gray.a);
}

TEST(ColorTest, CustomColor)
{
    Color c(123, 95, 1, 248);

    EXPECT_EQ(123, c.r);
    EXPECT_EQ(95,  c.g);
    EXPECT_EQ(1,   c.b);
    EXPECT_EQ(248, c.a);
}
