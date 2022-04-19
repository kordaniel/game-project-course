#include "Timer.hpp"
#include "Logger.hpp"

#include <ratio>


Timer::Timer(const std::string_view message)
    : _message(message)
    , _startTimepoint(std::chrono::steady_clock::now())
{
    //
}

Timer::Timer(void)
    : Timer("")
{
    //
}

Timer::~Timer(void)
{
    std::chrono::time_point<std::chrono::steady_clock> endTimepoint
        = std::chrono::steady_clock::now();
    processResult(endTimepoint);
}

void
Timer::Reset(void)
{
    _startTimepoint = std::chrono::steady_clock::now();
}

template<typename T>
int64_t Timer::Elapsed(void) const
{
    return std::chrono::duration_cast<T>(std::chrono::steady_clock::now() - _startTimepoint).count();
}


template int64_t Timer::Elapsed<std::chrono::nanoseconds> (void) const;
template int64_t Timer::Elapsed<std::chrono::microseconds>(void) const;
template int64_t Timer::Elapsed<std::chrono::milliseconds>(void) const;
template int64_t Timer::Elapsed<std::chrono::seconds>     (void) const;

// Private methods

void
Timer::processResult(const std::chrono::time_point<std::chrono::steady_clock>& endTimepoint) const
{
    using namespace std::literals::chrono_literals;
    using std::chrono::duration;

    const std::string_view* suffixPtr = nullptr;
    duration dur = endTimepoint - _startTimepoint;    
    double durAsDouble;    

    if (dur < 1us) {
        durAsDouble = duration<double, std::nano>(dur).count();
        suffixPtr = &Timer::MESSAGE_SUFFIXES[0];
    } else if (dur < 1ms) {
        durAsDouble = duration<double, std::micro>(dur).count();
        suffixPtr = &Timer::MESSAGE_SUFFIXES[1];
    } else if (dur < 1s) {
        durAsDouble = duration<double, std::milli>(dur).count();
        suffixPtr = &Timer::MESSAGE_SUFFIXES[2];
    } else {
        durAsDouble = duration<double, std::ratio<1>>(dur).count();
        suffixPtr = &Timer::MESSAGE_SUFFIXES[3];
    }

    if (_message.empty()) {
        Logger::Debug(Timer::MESSAGE_STANDARD, durAsDouble, *suffixPtr);
    } else {
        Logger::Debug(Timer::MESSAGE_FORMAT, _message, durAsDouble, *suffixPtr);
    }
}
