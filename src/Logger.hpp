#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string_view>
#include <string>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <ctime>


class Logger
{
public:
    enum class Level { TRACE = 0, DEBUG, INFO, CRITICAL };
    
    template<typename... Args>
    inline static void Trace(const std::string_view fmt, Args... args)
    { Logger::log(Level::TRACE, fmt, args...); }

    template<typename... Args>
    static void Debug(const std::string_view fmt, Args... args)
    { Logger::log(Level::DEBUG, fmt, args...); }

    template<typename... Args>
    static void Info(const std::string_view fmt, Args... args)
    { Logger::log(Level::INFO, fmt, args...); }

    template<typename... Args>
    static void Critical(const std::string_view fmt, Args... args)
    { Logger::log(Level::CRITICAL, fmt, args...); }

    static void               SetLogLevel(Logger::Level level);
    static Logger::Level      GetLogLevel(void);
    static const std::string& GetLogLevelAsString(void);
private:
    inline static const std::string s_levelNames[] = { "TRACE", "DEBUG", "INFO", "CRITICAL" };

#ifndef NDEBUG
    inline static Level             s_logLevel = Level::DEBUG;
#else
    inline static Level             s_logLevel = Level::INFO;
#endif

    template<typename... Args>
    static void log(Level priority, const std::string_view& fmt, Args... args)
    {
        if (s_logLevel > priority) {
            return;
        }

        fmt::print("[{:%T}] [{}]:\t{}\n",
            fmt::localtime(std::time(nullptr)),
            s_levelNames[static_cast<int>(priority)],
            fmt::format(fmt, args...)
        );
    }
};

#endif // LOGGER_HPP
