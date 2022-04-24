#ifndef TIMETOOLS_HPP
#define TIMETOOLS_HPP

#include <chrono>
#include <string_view>


/// A class representing a timestep in the game engine. Basically a wrapper for a double.
/// NOTE: All values default to seonds.
class Timestep
{
public:
    Timestep(double seconds = 0.0);
    Timestep(const Timestep& other) = default;
    Timestep(Timestep&& other)      = delete; // Move constructor deleted
    ~Timestep(void) = default;

    inline operator double(void) const { return _seconds; }
    inline operator float(void)  const { return static_cast<float>(_seconds); }

    bool operator<(const Timestep& rhs) const;
    bool operator>(const Timestep& rhs) const;

    void operator+=(const Timestep& rhs);
    void operator-=(const Timestep& rhs);
    void operator+=(double seconds);
    void operator-=(double seconds);

    bool   IsNonPositive(void)   const;
    double GetSeconds(void)      const;
    double GetMilliSeconds(void) const;

private:
    double _seconds;

};


/// A timer class. Can be used to time the execution time of scopes (often function calls) when
/// constructed with the default arguments utilizing the C++ RAII principle.
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
    int64_t Elapsed(bool resetTimer = false);

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


/// This class abstracts the timing functions needed to control the game loop.
/// NOTE: All values defaults to seconds.
class GameloopTimer
{
public:

    using Clock      = std::chrono::steady_clock;
    using Duration   = std::chrono::duration<double>;
    using Time_point = std::chrono::time_point<Clock, Duration>;

    /// Constructor. Pass in the arguments of target game state updates to be done per loop iteration along
    /// with the maximum time to spend updating the game state per loop iteration.
    /// @param iterationFreq Target amount of loop iterations per second, "FPS".
    /// @param updateFreq Target amount of game state updates to do per loop iteration.
    /// @param updateTimeMax Maximum time to spend updating the game state, per loop iteration.
    GameloopTimer(size_t iterationFreq, size_t updateFreq, double updateTimeMax);
    GameloopTimer(const GameloopTimer& other) = delete;
    GameloopTimer(GameloopTimer&& other)      = delete;
    ~GameloopTimer(void) = default;

    /// Initialize the timer for a new loop iteration, updates the accumulated lag with
    /// the time passed during the previous loop iteration.
    void InitIteration(void);

    /// Returns true while the accumulatedLag is greater or equal to the computed deltatime
    /// and max number of updates per loop iteratorion is not reached.
    /// @return true while the game state should be updated, false otherwise.
    bool ShouldDoUpdates(void);

    /// Returns the (fixed) target time for one loop iteration, which is computed from the iterationFreq ctor argument.
    Timestep GetIterationTargetTime(void) const;

    /// Returns the (fixed) deltatime, which is computed from the target update frequency.
    Timestep GetUpdateDeltaTime(void) const;

    /// Returns the accumulated lag, "interpolation" timestep for the renderer.
    Timestep GetLag(void) const;

    /// Returns the time delta between the target loop iteration time and the time spent in the loop.
    Timestep GetSleeptime(void) const;

private:
    inline static constexpr Duration DUR_ONE_SECOND = Duration(1.0);

    Duration   _targetTime; // The target time for one loop iteration.
    Duration   _dtUpdate;   // The delta time representing one timestep in the game simulation.
    Duration   _accumulatedLag;
    Time_point _timePrevious;
    Time_point _timeCurrent;

    size_t _updatesLimit; // Maximum number of updates to do per game loop iteration.
    size_t _updatesDone;  // Amount of updates done during this game loop iteration.

};

/// This class computes an ever more precise estimate of a duration, based on historical durations.
/// This is achieved by implementing Welford's online algorithm to compute the variance
/// between all durations in a single pass
/// https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford%27s_online_algorithm
class TimeEstimate
{
public:
    TimeEstimate(Timestep estimate, double mean = 0.0);
    TimeEstimate(const TimeEstimate& other) = delete;
    TimeEstimate(TimeEstimate&& other) = delete;
    ~TimeEstimate(void) = default;

    void AddTimestep(Timestep ts);

    // Returns the computed estimate. That is the Mean + stddev of the historical values.
    Timestep GetEstimate(void) const;
    size_t   GetCount(void)    const;

private:
    double _estimate;
    double _mean;
    double _m2;
    size_t _count;

};

namespace thread
{
    /// Sleeps in 1ms chunks, until the remaining time is less than the estimate, spinlocks the remaining time.
    /// This function achieves very good accuracy in contrast to the stdlib sleep_for function, with a
    /// inherited cost of cpu-usage overhead of ~3-5%.
    /// Inspiration: https://blat-blatnik.github.io/computerBear/making-accurate-sleep-function/
    /// @param seconds The timestep in seconds to sleep for.
    /// @param estimate The estimate that how long the sleeping actually takes, counting for overhead.
    void PreciseSleep(Timestep seconds, TimeEstimate& estimate);
} // end namespace thread

#endif // TIMETOOLS_HPP
