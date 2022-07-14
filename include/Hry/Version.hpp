/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <string>

#include <fmt/format.h>


namespace hry
{
/**
 * @brief General structure for version information
 * Usef for: API version, plugin version
 */
struct Version
{
    uint8_t major; // changes when api has breaking changes
    uint16_t minor;
    uint16_t patch;

    /**
     * @brief Converts structure to formatter string
     *
     * @return Formatted string
     */
    [[nodiscard]] std::string toString() const
    {
        return fmt::format("{}.{}.{}", major, minor, patch);
    }
};

/**
 * @brief Current API version
 */
constexpr inline Version ApiVersion{ 1, 0, 7 };
}

template<>
struct fmt::formatter<hry::Version>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(hry::Version const& version, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), "{}.{}.{}", version.major, version.minor, version.patch);
    }
};
