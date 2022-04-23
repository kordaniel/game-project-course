#include "gtest/gtest.h"
#include "gmock/gmock.h" //EXPECT_THAT macro, matchers

#include "Timetools.hpp"

TEST(Timestep, DefaultCreatedTimestepIsZero)
{
    Timestep ts;
    EXPECT_DOUBLE_EQ(static_cast<double>(ts), 0.0);
    EXPECT_FLOAT_EQ(static_cast<float>(ts), 0.0f);
    EXPECT_DOUBLE_EQ(ts.GetSeconds(), 0.0);
    EXPECT_DOUBLE_EQ(ts.GetMilliSeconds(), 0.0);
}

TEST(Timestep, OneSecond)
{
    Timestep ts = 1;
    EXPECT_DOUBLE_EQ(static_cast<double>(ts), 1.0);
    EXPECT_FLOAT_EQ(static_cast<float>(ts), 1.0f);
}

TEST(Timestep, Millisecond)
{
    Timestep ts(0.001);
    EXPECT_DOUBLE_EQ(ts.GetMilliSeconds(), 1.0);
    EXPECT_DOUBLE_EQ(ts.GetSeconds(), 0.001);
}

TEST(Timestep, Halfsecond)
{
    Timestep ts = 0.5;
    EXPECT_DOUBLE_EQ(ts.GetSeconds(), 0.5);
    EXPECT_DOUBLE_EQ(ts.GetMilliSeconds(), 500.0);
}

TEST(Timestep, OneThirdOfSecond)
{
    Timestep ts(1.0 / 3.0);
    EXPECT_DOUBLE_EQ(ts.GetSeconds(), 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(ts.GetMilliSeconds(), 1000.0 / 3.0);
}

TEST(GameloopTimer, 30UPSHalfSecondMax)
{
    constexpr double UPS = 30.0;
    GameloopTimer glt(UPS, 0.5);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 30UPSOneFifthSecondMax)
{
    constexpr double UPS = 30.0;
    GameloopTimer glt(UPS, 0.2);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 60UPSHalfSecondMax)
{
    constexpr double UPS = 60.0;
    GameloopTimer glt(UPS, 0.5);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 60UPSHalfOneFifthSecondMax)
{
    constexpr double UPS = 60.0;
    GameloopTimer glt(UPS, 0.2);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 120UPSHalfSecondMax)
{
    constexpr double UPS = 120.0;
    GameloopTimer glt(UPS, 0.5);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 120UPSOneFifthSecondMax)
{
    constexpr double UPS = 120.0;
    GameloopTimer glt(UPS, 0.2);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 144UPSHalfSecondMax)
{
    constexpr double UPS = 144.0;
    GameloopTimer glt(UPS, 0.5);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 144UPSOneFifthSecondMax)
{
    constexpr double UPS = 144.0;
    GameloopTimer glt(UPS, 0.2);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}
