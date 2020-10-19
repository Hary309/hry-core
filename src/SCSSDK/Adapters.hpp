/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <string>
#include <type_traits>

#include <scssdk_value.h>

#include "Hry/Math/SCSTypes.hpp"
#include "Hry/Namespace.hpp"

#include "SCSValueType.hpp"

HRY_NS_BEGIN

template<typename T>
using ValueAdapter_t = T (*)(const scs_value_t& param) noexcept;

template<typename T>
inline T valueAdapter(const scs_value_t& param) noexcept = delete;

template<>
inline bool valueAdapter<bool>(const scs_value_t& param) noexcept
{
    if (param.type == SCS_VALUE_TYPE_bool)
    {
        return param.value_bool.value != 0;
    }
    return false;
}

template<>
inline int32_t valueAdapter<int32_t>(const scs_value_t& param) noexcept
{
    if (param.type == SCS_VALUE_TYPE_s32)
    {
        return param.value_s32.value;
    }
    return 0;
}

template<>
inline uint32_t valueAdapter<uint32_t>(const scs_value_t& param) noexcept
{
    if (param.type == SCS_VALUE_TYPE_u32)
    {
        return param.value_u32.value;
    }
    return 0;
}

template<>
inline int64_t valueAdapter<int64_t>(const scs_value_t& param) noexcept
{
    if (param.type == SCS_VALUE_TYPE_s64)
    {
        return param.value_s64.value;
    }
    return 0;
}

template<>
inline uint64_t valueAdapter<uint64_t>(const scs_value_t& param) noexcept
{
    if (param.type == SCS_VALUE_TYPE_u64)
    {
        return param.value_u64.value;
    }
    return 0;
}

template<>
inline float valueAdapter<float>(const scs_value_t& param) noexcept
{
    if (param.type == SCS_VALUE_TYPE_float)
    {
        return param.value_float.value;
    }
    return 0;
}

template<>
inline double valueAdapter<double>(const scs_value_t& param) noexcept
{
    if (param.type == SCS_VALUE_TYPE_double)
    {
        return param.value_double.value;
    }
    return 0;
}

template<>
inline Vec3<float> valueAdapter<Vec3<float>>(const scs_value_t& param) noexcept
{
    if (param.type == SCS_VALUE_TYPE_fvector)
    {
        const auto& vec = param.value_fvector;
        return { vec.x, vec.y, vec.z };
    }
    return {};
}

template<>
inline Vec3<double> valueAdapter<Vec3<double>>(const scs_value_t& param) noexcept
{
    if (param.type == SCS_VALUE_TYPE_dvector)
    {
        const auto& vec = param.value_dvector;
        return { vec.x, vec.y, vec.z };
    }
    return {};
}

template<>
inline Euler valueAdapter<Euler>(const scs_value_t& param) noexcept
{
    if (param.type == SCS_VALUE_TYPE_euler)
    {
        const auto& euler = param.value_euler;
        return { euler.heading, euler.pitch, euler.roll };
    }
    return {};
}

template<>
inline PlacementF valueAdapter<PlacementF>(const scs_value_t& param) noexcept
{
    if (param.type == SCS_VALUE_TYPE_fplacement)
    {
        const auto& pl = param.value_fplacement;
        return { { pl.position.x, pl.position.y, pl.position.z },
                 { pl.orientation.heading, pl.orientation.pitch, pl.orientation.roll } };
    }
    return {};
}

template<>
inline PlacementD valueAdapter<PlacementD>(const scs_value_t& param) noexcept
{
    if (param.type == SCS_VALUE_TYPE_dplacement)
    {
        const auto& pl = param.value_dplacement;
        return { { pl.position.x, pl.position.y, pl.position.z },
                 { pl.orientation.heading, pl.orientation.pitch, pl.orientation.roll } };
    }
    return {};
}

template<>
inline std::string valueAdapter<std::string>(const scs_value_t& param) noexcept
{
    if (param.type == SCS_VALUE_TYPE_string)
    {
        return param.value_string.value;
    }
    return {};
}

template<typename T>
auto CreateAdapter()
{
    return &valueAdapter<T>;
}

template<typename T>
inline constexpr auto HasAdapter_v = SCSValueType_v<T> != SCS_VALUE_TYPE_INVALID;

HRY_NS_END
