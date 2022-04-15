#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string_view>
#include <string>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <fmt/color.h>
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

    inline static Level             s_logLevel =
#ifndef NDEBUG
     Level::DEBUG;
#else
    Level::INFO;
#endif

    template<typename... Args>
    static void log(Level priority, const std::string_view& fmt, Args... args)
    {
        if (s_logLevel > priority) {
            return;
        }

        static constexpr char placeholderTime[] = "[{:%T}]:";
        static constexpr char placeholderMesg[] = "[{}]:\t{}\n";

        // Print time
        fmt::print(placeholderTime, fmt::localtime(std::time(nullptr)));

        // Print message, switch color on message priority. Use terminal default for INFO messages
        switch (priority)
        {
            case Level::TRACE:
                fmt::print(fg(fmt::terminal_color::green),
                    placeholderMesg,
                    s_levelNames[static_cast<size_t>(priority)],
                    fmt::format(fmt, args...)
                );
                break;
            case Level::DEBUG:
                fmt::print(fg(fmt::terminal_color::yellow),
                    placeholderMesg,
                    s_levelNames[static_cast<size_t>(priority)],
                    fmt::format(fmt, args...)
                );
                break;
            case Level::INFO:
                fmt::print(
                    placeholderMesg,
                    s_levelNames[static_cast<size_t>(priority)],
                    fmt::format(fmt, args...)
                );
                break;
            case Level::CRITICAL:
                fmt::print(fmt::emphasis::bold | fg(fmt::terminal_color::red),
                    placeholderMesg,
                    s_levelNames[static_cast<size_t>(priority)],
                    fmt::format(fmt, args...)
                );
                break;
        default:
            break;
        }
    }
};

#endif // LOGGER_HPP
