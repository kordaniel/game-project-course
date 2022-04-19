#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <string_view>


class Timer
{
public:
    Timer(const std::string_view message, bool autoLogElapsedAtDestruction = true);
    Timer(bool autoLogElapsedAtDestruction = true);
    Timer(const Timer& other) = delete; // Copy constructor
    Timer(Timer&& other)      = delete; // Move constructor
    ~Timer(void);

    void Reset(void);

    template<typename T>
    int64_t Elapsed(void) const;

private:
    void processResult(const std::chrono::time_point<std::chrono::steady_clock>& endTimepoint) const;

private:
    inline static constexpr std::string_view MESSAGE_STANDARD    = "Timer ran for {}{}.";
    inline static constexpr std::string_view MESSAGE_FORMAT      = "{}: {}{}.";
    inline static constexpr std::string_view MESSAGE_SUFFIXES[4] = { "ns", "us", "ms", "s" };

    std::chrono::time_point<std::chrono::steady_clock> _startTimepoint;
    const std::string_view                             _message;
    bool                                               _autoLog;

};

#endif // TIMER_HPP
