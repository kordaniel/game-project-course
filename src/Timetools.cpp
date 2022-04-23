#include "Timetools.hpp"
#include "Logger.hpp"

#include <ratio>

Timestep::Timestep(double seconds)
    : _seconds(seconds)
{
    //
}

double
Timestep::GetSeconds(void) const { return _seconds; }

double
Timestep::GetMilliSeconds(void) const { return 1000.0 * _seconds; }


Timer::Timer(const std::string_view message, bool autoLogElapsedAtDestruction)
    : _startTimepoint(std::chrono::steady_clock::now())
    , _message(message)
    , _autoLog(autoLogElapsedAtDestruction)
{
    //
}

Timer::Timer(bool autoLogElapsedAtDestruction)
    : Timer("", autoLogElapsedAtDestruction)
{
    //
}

Timer::~Timer(void)
{
    std::chrono::time_point<std::chrono::steady_clock> endTimepoint
        = std::chrono::steady_clock::now();
    if (_autoLog) {
        processResult(endTimepoint);
    }
}

void
Timer::Reset(void)
{
    _startTimepoint = std::chrono::steady_clock::now();
}

template<typename T>
int64_t Timer::Elapsed(bool resetTimer)
{
    int64_t elapsedVal = std::chrono::duration_cast<T>(
        std::chrono::steady_clock::now() - _startTimepoint
    ).count();

    if (resetTimer) {
        Reset();
    }

    return elapsedVal;
}


template int64_t Timer::Elapsed<std::chrono::nanoseconds> (bool resetTimer);
template int64_t Timer::Elapsed<std::chrono::microseconds>(bool resetTimer);
template int64_t Timer::Elapsed<std::chrono::milliseconds>(bool resetTimer);
template int64_t Timer::Elapsed<std::chrono::seconds>     (bool resetTimer);

void
Timer::processResult(const std::chrono::time_point<std::chrono::steady_clock>& endTimepoint) const
{ // Private method
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

GameloopTimer::GameloopTimer(size_t updateFreq, double updateTimeMax)
    : _dtUpdate(DUR_ONE_SECOND / updateFreq)
    , _accumulatedLag(Duration(0.0))
    , _timePrevious(Clock::now())
    , _timeCurrent(Clock::now())
    , _updatesLimit(static_cast<size_t>(updateTimeMax * DUR_ONE_SECOND / _dtUpdate))
    , _updatesDone(0)
{
    //
}

void
GameloopTimer::InitIteration(void)
{
    _timeCurrent     = Clock::now();
    _accumulatedLag += (_timeCurrent - _timePrevious);
    _timePrevious    = _timeCurrent;
    _updatesDone     = 0;
}

bool
GameloopTimer::ShouldDoUpdates(void)
{
    if (_accumulatedLag < _dtUpdate) {
        return false;
    }

    if (++_updatesDone > _updatesLimit)
    {
        Logger::Critical("Updates lagging behind..");
        return false;
    }

    _accumulatedLag -= _dtUpdate;

    return true;
}

Timestep
GameloopTimer::GetUpdateDeltaTime(void) const
{
    return { _dtUpdate.count() };
}

Timestep
GameloopTimer::GetLag(void) const
{
    return { _accumulatedLag.count() };
}
