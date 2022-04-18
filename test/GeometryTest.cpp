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
