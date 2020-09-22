#pragma once

#include <variant>

#include <scssdk_telemetry.h>
#include <scssdk_value.h>
#include <stdint.h>

#include "Hry/Math/SCSTypes.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

template<typename>
struct SCSValueType
{
    static constexpr auto Value = SCS_VALUE_TYPE_INVALID;
};

template<>
struct SCSValueType<bool>
{
    static constexpr auto Value = SCS_VALUE_TYPE_bool;
};

template<>
struct SCSValueType<int32_t>
{
    static constexpr auto Value = SCS_VALUE_TYPE_s32;
};

template<>
struct SCSValueType<uint32_t>
{
    static constexpr auto Value = SCS_VALUE_TYPE_u32;
};

template<>
struct SCSValueType<uint64_t>
{
    static constexpr auto Value = SCS_VALUE_TYPE_u64;
};

template<>
struct SCSValueType<float>
{
    static constexpr auto Value = SCS_VALUE_TYPE_float;
};

template<>
struct SCSValueType<double>
{
    static constexpr auto Value = SCS_VALUE_TYPE_double;
};

template<>
struct SCSValueType<Vec3<float>>
{
    static constexpr auto Value = SCS_VALUE_TYPE_fvector;
};

template<>
struct SCSValueType<Vec3<double>>
{
    static constexpr auto Value = SCS_VALUE_TYPE_dvector;
};

template<>
struct SCSValueType<Euler>
{
    static constexpr auto Value = SCS_VALUE_TYPE_euler;
};

template<>
struct SCSValueType<PlacementF>
{
    static constexpr auto Value = SCS_VALUE_TYPE_fplacement;
};

template<>
struct SCSValueType<PlacementD>
{
    static constexpr auto Value = SCS_VALUE_TYPE_dplacement;
};

template<>
struct SCSValueType<std::string>
{
    static constexpr auto Value = SCS_VALUE_TYPE_string;
};

template<typename T>
constexpr inline auto SCSValueType_v = SCSValueType<T>::Value;

HRY_NS_END
