#include "Timetools.hpp"
#include "Logger.hpp"

#include <cmath>
#include <ratio>
#include <thread>


Timestep::Timestep(double seconds)
    : _seconds(seconds)
{
    //
}

bool
Timestep::operator<(const Timestep& rhs) const { return _seconds < rhs._seconds; }

bool
Timestep::operator>(const Timestep& rhs) const { return rhs < *this; }

void
Timestep::operator+=(const Timestep& rhs) { _seconds += rhs._seconds; }

void
Timestep::operator-=(const Timestep& rhs) { _seconds -= rhs._seconds; }

void
Timestep::operator+=(double seconds) { _seconds += seconds; }

void
Timestep::operator-=(double seconds) { _seconds -= seconds; }

bool
Timestep::IsNonPositive(void) const { return !(_seconds > 0.0); }

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


GameloopTimer::GameloopTimer(size_t iterationFreq, size_t updateFreq, double updateTimeMax)
    : _targetTime(DUR_ONE_SECOND / iterationFreq)
    , _dtUpdate(DUR_ONE_SECOND / updateFreq)
    , _accumulatedLag(Duration(0.0))
    , _timePrevious(Clock::now())
    , _timeCurrent(Clock::now())
    , _updatesLimit(static_cast<size_t>(updateTimeMax * DUR_ONE_SECOND / _dtUpdate))
    , _updatesDone(0)
{
    //
}

void
GameloopTimer::ResetFields(void)
{
    _accumulatedLag = Duration(0.0);
    _timePrevious = Clock::now();
    _timeCurrent = Clock::now();
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
GameloopTimer::GetIterationTargetTime(void) const
{
    return { _targetTime.count() };
}

Timestep
GameloopTimer::GetIterationElapsedTime(void) const
{
    return { Duration{Clock::now() - _timeCurrent}.count() };
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

Timestep
GameloopTimer::GetSleeptime(void) const
{
    return { (_targetTime - Duration{Clock::now() - _timeCurrent}).count() };
}


TimeEstimate::TimeEstimate(Timestep estimate, double mean)
    : _estimate(estimate)
    , _mean(mean)
    , _m2(0.0)
    , _count(1)
{
    //
}

void
TimeEstimate::AddTimestep(Timestep ts)
{
    double delta = static_cast<double>(ts) - _mean;

    ++_count;
    _mean += delta / static_cast<double>(_count);
    _m2   += delta * (static_cast<double>(ts) - _mean);
    _estimate = _mean + std::sqrt(_m2 / static_cast<double>(_count - 1)); // mean + stddev
}

Timestep
TimeEstimate::GetEstimate(void) const
{
    return { _estimate };
}

size_t
TimeEstimate::GetCount(void) const { return _count; }


void
thread::PreciseSleep(Timestep seconds, TimeEstimate& estimate)
{
    if (seconds.IsNonPositive()) {
        Logger::Debug("Unable to sleep for a negative period of time");
        return;
    }
    using std::chrono::milliseconds;
    using Clock      = std::chrono::steady_clock;
    using Duration   = std::chrono::duration<double>;
    using Time_point = std::chrono::time_point<Clock, Duration>;

    // Sleep in ~1ms chuncks until the remaining sleeping time gets smaller
    // than the estimated actual sleeping time counting for OS scheduling.
    while (seconds > estimate.GetEstimate())
    {
        Time_point startTime = Clock::now();
        std::this_thread::sleep_for(milliseconds(1));
        Time_point endTime = Clock::now();
        Timestep sleepDuration{ (endTime - startTime).count() };
        seconds -= sleepDuration;
        estimate.AddTimestep(sleepDuration);
    }

    // Spinlock the remaining time.
    Time_point startTime = Clock::now();
    while ((Time_point{Clock::now()} - startTime).count() < static_cast<double>(seconds));
}
