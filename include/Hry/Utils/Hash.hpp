/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

/// Author: github.com/MetGang

#pragma once
#include <cstdint>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

using Hash64_t = uint64_t;

template<typename InputIt>
constexpr Hash64_t FNV1a_64(InputIt begin) noexcept
{
    Hash64_t value = 0xCBF29CE484222325ull;

    while (*begin)
    {
        value ^= *begin;
        value *= 0x100000001B3ull;
        ++begin;
    }

    return value;
}

HRY_NS_END
