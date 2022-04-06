#include "gtest/gtest.h"
#include "gmock/gmock.h" //EXPECT_THAT macro, matchers

#include "Logger.hpp"


TEST(LoggerTest, HasCorrectLevels)
{
    EXPECT_EQ(static_cast<int>(Logger::Level::TRACE),    0);
    EXPECT_EQ(static_cast<int>(Logger::Level::DEBUG),    1);
    EXPECT_EQ(static_cast<int>(Logger::Level::INFO),     2);
    EXPECT_EQ(static_cast<int>(Logger::Level::CRITICAL), 3);
}

TEST(LoggerTest, LevelCanBeSet)
{
    Logger::SetLogLevel(Logger::Level::TRACE);
    EXPECT_EQ(static_cast<int>(Logger::GetLogLevel()), 0);

    Logger::SetLogLevel(Logger::Level::DEBUG);
    EXPECT_EQ(static_cast<int>(Logger::GetLogLevel()), 1);

    Logger::SetLogLevel(Logger::Level::INFO);
    EXPECT_EQ(static_cast<int>(Logger::GetLogLevel()), 2);

    Logger::SetLogLevel(Logger::Level::CRITICAL);
    EXPECT_EQ(static_cast<int>(Logger::GetLogLevel()), 3);
}

TEST(LoggerTest, LevelTraceLogsAll)
{
    using ::testing::HasSubstr;
    
    testing::internal::CaptureStdout();
    Logger::SetLogLevel(Logger::Level::TRACE);
    Logger::Critical("Critical message");
    Logger::Info("Informational string");
    Logger::Debug("Debug print");
    Logger::Trace("Trace can print a very long string");

    EXPECT_THAT(testing::internal::GetCapturedStdout(), AllOf(
        HasSubstr("Critical message"),
        HasSubstr("Informational string"),
        HasSubstr("Debug print"),
        HasSubstr("Trace can print a very long string")
    ));
}

TEST(LoggerTest, LevelDebugLogsDebugAndHigher)
{
    using ::testing::HasSubstr;

    testing::internal::CaptureStdout();
    Logger::SetLogLevel(Logger::Level::DEBUG);
    Logger::Critical("Critical message");
    Logger::Info("Informational string");
    Logger::Debug("Debug print");
    Logger::Trace("Trace can print a very long string");

    EXPECT_THAT(testing::internal::GetCapturedStdout(), AllOf(
        HasSubstr("Critical message"),
        HasSubstr("Informational string"),
        HasSubstr("Debug print"),
        Not(HasSubstr("Trace can print a very long string"))
    ));
}

TEST(LoggerTest, LevelInfoLogsInfoAndHigher)
{
    using ::testing::HasSubstr;

    testing::internal::CaptureStdout();
    Logger::SetLogLevel(Logger::Level::INFO);
    Logger::Critical("Critical message");
    Logger::Info("Informational string");
    Logger::Debug("Debug print");
    Logger::Trace("Trace can print a very long string");

    EXPECT_THAT(testing::internal::GetCapturedStdout(), AllOf(
        HasSubstr("Critical message"),
        HasSubstr("Informational string"),
        Not(HasSubstr("Debug print")),
        Not(HasSubstr("Trace can print a very long string"))
    ));
}

TEST(LoggerTest, LevelCriticalLogsCriticalOnly)
{
    using ::testing::HasSubstr;

    testing::internal::CaptureStdout();
    Logger::SetLogLevel(Logger::Level::CRITICAL);
    Logger::Critical("Critical message");
    Logger::Info("Informational string");
    Logger::Debug("Debug print");
    Logger::Trace("Trace can print a very long string");

    EXPECT_THAT(testing::internal::GetCapturedStdout(), AllOf(
        HasSubstr("Critical message"),
        Not(HasSubstr("Informational string")),
        Not(HasSubstr("Debug print")),
        Not(HasSubstr("Trace can print a very long string"))
    ));
}
