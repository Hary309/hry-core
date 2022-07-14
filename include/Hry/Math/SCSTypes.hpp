/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Vec3.hpp"

#include <type_traits>

namespace hry
{
/**
 * @brief Orientation of object.
 */
struct Euler
{
    /**
     * @brief Heading.
     *
     * Stored in unit range where <0,1) corresponds to <0,360).
     *
     * The angle is measured counterclockwise in horizontal plane when looking
     * from top where 0 corresponds to forward (north), 0.25 to left (west),
     * 0.5 to backward (south) and 0.75 to right (east).
     */
    float heading;

    /**
     * @brief Pitch
     *
     * Stored in unit range where <-0.25,0.25> corresponds to <-90,90>.
     *
     * The pitch angle is zero when in horizontal direction,
     * with positive values pointing up (0.25 directly to zenith),
     * and negative values pointing down (-0.25 directly to nadir).
     */
    float pitch;

    /**
     * @brief Roll
     *
     * Stored in unit range where <-0.5,0.5> corresponds to <-180,180>.
     *
     * The angle is measured in counterclockwise when looking in direction of
     * the roll axis.
     */
    float roll;
};

/**
 * @name Combination of position and orientation.
 */
struct PlacementF
{
    Vec3<float> position;
    Euler orientation;
};

/**
 * @name Combination of position and orientation.
 */
struct PlacementD
{
    Vec3<double> position;
    Euler orientation;
};
}

namespace fmt
{
template<>
struct formatter<hry::Euler>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(hry::Euler const& vec, FormatContext& ctx)
    {
        return format_to(ctx.out(), "Euler: {{{}, {}, {}}}", vec.heading, vec.pitch, vec.roll);
    }
};

template<>
struct formatter<hry::PlacementF>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(hry::PlacementF const& vec, FormatContext& ctx)
    {
        return format_to(ctx.out(), "PlacementF: {{{}, {}}}", vec.position, vec.orientation);
    }
};

template<>
struct formatter<hry::PlacementD>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(hry::PlacementD const& vec, FormatContext& ctx)
    {
        return format_to(ctx.out(), "PlacementD: {{{}, {}}}", vec.position, vec.orientation);
    }
};
}
