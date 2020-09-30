/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <string>

#include <fmt/format.h>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

struct Version
{
    uint8_t major; // changes when api have breaking changes
    uint16_t minor;
    uint16_t patch;

    [[nodiscard]] std::string toString() const
    {
        return fmt::format("{}.{}.{}", major, minor, patch);
    }
};

constexpr inline Version ApiVersion{ 0, 1, 0 };

inline bool IsApiCompatible(Version version)
{
    return version.major == ApiVersion.major;
}

HRY_NS_END
