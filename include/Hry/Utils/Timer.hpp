#pragma once

#include <chrono>

namespace hry
{

class Timer
{
public:
    using Clock_t = std::chrono::high_resolution_clock;
    using TimePoint_t = Clock_t::time_point;
    using Seconds_t = std::chrono::duration<float, std::ratio<1>>;
    using Milliseconds_t = std::chrono::milliseconds;
    using Microseconds_t = std::chrono::microseconds;

private:
    TimePoint_t _start;

public:
    Timer() : _start(Clock_t::now()) { }

    void reset()
    {
        _start = Clock_t::now();
    }

    auto elapsed() const
    {
        return Clock_t::now() - _start;
    }

    float asSeconds() const 
    {
        return std::chrono::duration_cast<Seconds_t>(elapsed()).count();
    }

    int64_t asMiliseconds() const 
    {
        return std::chrono::duration_cast<Milliseconds_t>(elapsed()).count();
    }

    int64_t asMicrosecond() const 
    {
        return std::chrono::duration_cast<Microseconds_t>(elapsed()).count();
    }
};

}
