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

TEST(GameloopTimer, 30FPS)
{
    constexpr size_t FPS = 30;
    GameloopTimer glt(FPS, 20, 0.5);
    EXPECT_DOUBLE_EQ(glt.GetIterationTargetTime().GetSeconds(), 1. / FPS);
    EXPECT_DOUBLE_EQ(glt.GetIterationTargetTime().GetMilliSeconds(), 1000. / FPS);
}

TEST(GameloopTimer, 60FPS)
{
    constexpr size_t FPS = 60;
    GameloopTimer glt(FPS, 30, 0.5);
    EXPECT_DOUBLE_EQ(glt.GetIterationTargetTime().GetSeconds(), 1. / FPS);
    EXPECT_DOUBLE_EQ(glt.GetIterationTargetTime().GetMilliSeconds(), 1000. / FPS);
}

TEST(GameloopTimer, 90FPS)
{
    constexpr size_t FPS = 90;
    GameloopTimer glt(FPS, 90, 0.5);
    EXPECT_DOUBLE_EQ(glt.GetIterationTargetTime().GetSeconds(), 1. / FPS);
    EXPECT_DOUBLE_EQ(glt.GetIterationTargetTime().GetMilliSeconds(), 1000. / FPS);
}

TEST(GameloopTimer, 120FPS)
{
    constexpr size_t FPS = 120;
    GameloopTimer glt(FPS, 30, 0.5);
    EXPECT_DOUBLE_EQ(glt.GetIterationTargetTime().GetSeconds(), 1. / FPS);
    EXPECT_DOUBLE_EQ(glt.GetIterationTargetTime().GetMilliSeconds(), 1000. / FPS);
}

TEST(GameloopTimer, 144FPS)
{
    constexpr size_t FPS = 144;
    GameloopTimer glt(FPS, 30, 0.5);
    EXPECT_DOUBLE_EQ(glt.GetIterationTargetTime().GetSeconds(), 1. / FPS);
    EXPECT_DOUBLE_EQ(glt.GetIterationTargetTime().GetMilliSeconds(), 1000. / FPS);
}

TEST(GameloopTimer, 30UPSHalfSecondMax)
{
    constexpr size_t UPS = 30.0;
    GameloopTimer glt(30, UPS, 0.5);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 30UPSOneFifthSecondMax)
{
    constexpr size_t UPS = 30.0;
    GameloopTimer glt(60, UPS, 0.2);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 60UPSHalfSecondMax)
{
    constexpr size_t UPS = 60.0;
    GameloopTimer glt(30, UPS, 0.5);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 60UPSHalfOneFifthSecondMax)
{
    constexpr size_t UPS = 60.0;
    GameloopTimer glt(60, UPS, 0.2);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 120UPSHalfSecondMax)
{
    constexpr size_t UPS = 120.0;
    GameloopTimer glt(120, UPS, 0.5);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 120UPSOneFifthSecondMax)
{
    constexpr size_t UPS = 120.0;
    GameloopTimer glt(60, UPS, 0.2);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 144UPSHalfSecondMax)
{
    constexpr size_t UPS = 144.0;
    GameloopTimer glt(10, UPS, 0.5);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}

TEST(GameloopTimer, 144UPSOneFifthSecondMax)
{
    constexpr size_t UPS = 144.0;
    GameloopTimer glt(25, UPS, 0.2);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetSeconds(), 1. / UPS);
    EXPECT_DOUBLE_EQ(glt.GetUpdateDeltaTime().GetMilliSeconds(), 1000. / UPS);
}
