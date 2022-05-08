#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "Timetools.hpp"

#include <string>
#include <string_view>
#include <fmt/core.h>
#include <fmt/chrono.h>
#include <fmt/color.h>
//#include <fmt/format.h>
#include <ctime>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp>


template<>
struct fmt::formatter<Timestep>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Timestep ts, FormatContext& ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "{}", static_cast<double>(ts));
    }
};

template<>
struct fmt::formatter<glm::vec3>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(glm::vec3 const& vec3, FormatContext& ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "{}", glm::to_string(vec3));
    }
};

template<>
struct fmt::formatter<glm::vec4>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(glm::vec4 const& vec4, FormatContext& ctx) -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "{}", glm::to_string(vec4));
    }
};

template<>
struct fmt::formatter<glm::mat4>
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.begin();
    }

    /// Formats the glm::mat4 as a row ordered matrix, in contrast to glm:s internal column order.
    template<typename FormatContext>
    auto format(glm::mat4 const& mat4, FormatContext& ctx) -> decltype(ctx.out())
    {
        const std::string formatRow = "{: > 15.9f},{: > 15.9f},{: > 15.9f},{: > 15.9f}\n";
        const std::string formatStr =
            "mat4x4(\n" +
            formatRow +
            formatRow +
            formatRow +
            formatRow + ")";

        return fmt::format_to(
            ctx.out(),
            formatStr,
            mat4[0][0], mat4[1][0], mat4[2][0], mat4[3][0],
            mat4[0][1], mat4[1][1], mat4[2][1], mat4[3][1],
            mat4[0][2], mat4[1][2], mat4[2][2], mat4[3][2],
            mat4[0][3], mat4[1][3], mat4[2][3], mat4[3][3]
        );
    }
};


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
