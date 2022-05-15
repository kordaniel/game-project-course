#include "gtest/gtest.h"
#include "gmock/gmock.h" //EXPECT_THAT macro, matchers

#include "Geometry.hpp"


TEST(Dimensions2DTest, EqualsOperatorReturnsTrueWhenEqual)
{
    Dimensions2D l = { 1, 2 };
    Dimensions2D r = { 1, 2 };

    EXPECT_TRUE(l == r);
}

TEST(Dimensions2DTest, EqualsOperatorReturnsFalseWhenDiffers)
{
    Dimensions2D l = {  1,  2 };
    Dimensions2D r = { -1, -2 };

    EXPECT_FALSE(l == r);
}

TEST(Dimensions2DTest, EqualsOperatorReturnsFalseWhenWidthDiffers)
{
    Dimensions2D l = { 2, 2 };
    Dimensions2D r = { 4, 2 };

    EXPECT_FALSE(l == r);
}

TEST(Dimensions2DTest, EqualsOperatorReturnsFalseWhenHeightDiffers)
{
    Dimensions2D l = { 0, 1 };
    Dimensions2D r = { 0, 2 };

    EXPECT_FALSE(l == r);
}

TEST(Dimensions2DTest, NotEqualsOperatorReturnsTrueWhenDiffers)
{
    Dimensions2D l = { 2, 3 };
    Dimensions2D r = { 5, 7 };
    EXPECT_TRUE(l != r);
}

TEST(Dimensions2DTest, NotEqualsOperatorReturnsTrueWhenWidthDiffers)
{
    Dimensions2D l = { 0, 123 };
    Dimensions2D r = { 1, 123 };
    EXPECT_TRUE(l != r);
}

TEST(Dimensions2DTest, NotEqualsOperatorReturnsTrueWhenHeightDiffers)
{
    Dimensions2D l = { 10, 1231 };
    Dimensions2D r = { 10, 10 };
    EXPECT_TRUE(l != r);
}

TEST(Dimensions2DTest, NotEqualsOperatorReturnsFalseWhenEqual)
{
    Dimensions2D l = { 2321, 0 };
    Dimensions2D r = { 2321, 0 };
    EXPECT_FALSE(l != r);
}

TEST(RectangleFTest, EqualRectsOverlapsReturnTrue)
{
    RectangleF rectA = { 10.0f, 20.0f, 100.0f, 100.0f};
    RectangleF rectB = { 10.0f, 20.0f, 100.0f, 100.0f};
    EXPECT_TRUE(rectA.Overlaps(rectB));
    EXPECT_TRUE(rectB.Overlaps(rectA));
}

TEST(RectangleFTest, NonOverlappingRectsReturnFalse)
{
    constexpr float x = 10.0f;
    constexpr float y = 20.0f;
    constexpr float w = 55.3f;
    constexpr float h = 242.01f;
    constexpr float e = 0.00001f;

    RectangleF rectA = { x, y, w, h};
    RectangleF rectB = { x + w + e, y + h + e, w, h };

    EXPECT_FALSE(rectA.Overlaps(rectB));
    EXPECT_FALSE(rectB.Overlaps(rectA));
}
