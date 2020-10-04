/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <chrono>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class Timer final
{
public:
    using Clock_t = std::chrono::high_resolution_clock;
    using TimePoint_t = Clock_t::time_point;
    using Seconds_t = std::chrono::duration<double, std::ratio<1>>;
    using Milliseconds_t = std::chrono::milliseconds;
    using Microseconds_t = std::chrono::microseconds;

private:
    TimePoint_t _start;

public:
    Timer() : _start(Clock_t::now()) {}

    void reset() { _start = Clock_t::now(); }

    [[nodiscard]] auto elapsed() const { return Clock_t::now() - _start; }

    [[nodiscard]] double asSeconds() const
    {
        return std::chrono::duration_cast<Seconds_t>(elapsed()).count();
    }

    [[nodiscard]] int64_t asMiliseconds() const
    {
        return std::chrono::duration_cast<Milliseconds_t>(elapsed()).count();
    }

    [[nodiscard]] int64_t asMicrosecond() const
    {
        return std::chrono::duration_cast<Microseconds_t>(elapsed()).count();
    }
};

HRY_NS_END
