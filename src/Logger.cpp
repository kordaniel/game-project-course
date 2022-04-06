#include "Logger.hpp"


// Static functions
void
Logger::SetLogLevel(Logger::Level level)
{
    s_logLevel = level;
}

Logger::Level
Logger::GetLogLevel(void)
{
    return s_logLevel;
}

const std::string&
Logger::GetLogLevelAsString(void)
{
    return s_levelNames[static_cast<size_t>(s_logLevel)];
}
// End static functions
