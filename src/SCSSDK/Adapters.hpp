#pragma once

#include <string>
#include <type_traits>

#include "Hry/Math/SCSTypes.hpp"

#include <scssdk_value.h>

template<typename>
struct BasicAdapterCreator;

/////////// -- bool -- ///////////
struct BoolAdapter
{
    using ObjectType = bool;

    bool get(const scs_value_t& param) const
    {
        if (param.type == SCS_VALUE_TYPE_bool)
        {
            return param.value_bool.value != 0;
        }
        return false;
    }
};

template<>
struct BasicAdapterCreator<bool>
{
    static auto create() { return BoolAdapter(); }
};

/////////// -- int32 -- ///////////
struct Int32Adapter
{
    using ObjectType = int32_t;

    int32_t get(const scs_value_t& param) const
    {
        if (param.type == SCS_VALUE_TYPE_s32)
        {
            return param.value_s32.value;
        }
        return 0;
    }
};

template<>
struct BasicAdapterCreator<int32_t>
{
    static auto create() { return Int32Adapter(); }
};

/////////// -- unsigned int32 -- ///////////
struct UInt32Adapter
{
    using ObjectType = uint32_t;

    uint32_t get(const scs_value_t& param) const
    {
        if (param.type == SCS_VALUE_TYPE_u32)
        {
            return param.value_u32.value;
        }
        return 0;
    }
};

template<>
struct BasicAdapterCreator<uint32_t>
{
    static auto create() { return UInt32Adapter(); }
};

/////////// -- int64 -- ///////////
struct Int64Adapter
{
    using ObjectType = int64_t;

    int64_t get(const scs_value_t& param) const
    {
        if (param.type == SCS_VALUE_TYPE_s64)
        {
            return param.value_s64.value;
        }
        return 0;
    }
};

template<>
struct BasicAdapterCreator<int64_t>
{
    static auto create() { return Int64Adapter(); }
};

/////////// -- unsigned int64 -- ///////////
struct UInt64Adapter
{
    using ObjectType = uint64_t;

    uint64_t get(const scs_value_t& param) const
    {
        if (param.type == SCS_VALUE_TYPE_u64)
        {
            return param.value_u64.value;
        }
        return 0;
    }
};

template<>
struct BasicAdapterCreator<uint64_t>
{
    static auto create() { return UInt64Adapter(); }
};

/////////// -- Float -- ///////////
struct FloatAdapter
{
    using ObjectType = float;

    float get(const scs_value_t& param) const
    {
        if (param.type == SCS_VALUE_TYPE_float)
        {
            return param.value_float.value;
        }
        return 0;
    }
};

template<>
struct BasicAdapterCreator<float>
{
    static auto create() { return FloatAdapter(); }
};

/////////// -- Double -- ///////////
struct DoubleAdapter
{
    using ObjectType = double;

    double get(const scs_value_t& param) const
    {
        if (param.type == SCS_VALUE_TYPE_double)
        {
            return param.value_double.value;
        }
        return 0;
    }
};

template<>
struct BasicAdapterCreator<double>
{
    static auto create() { return DoubleAdapter(); }
};

/////////// -- Vec3<float> -- ///////////
struct Vec3FloatAdapter
{
    using ObjectType = hry::Vec3<float>;

    hry::Vec3<float> get(const scs_value_t& param) const
    {
        if (param.type == SCS_VALUE_TYPE_fvector)
        {
            const auto& vec = param.value_fvector;
            return { vec.x, vec.y, vec.z };
        }
        return {};
    }
};

template<>
struct BasicAdapterCreator<hry::Vec3<float>>
{
    static auto create() { return Vec3FloatAdapter(); }
};

/////////// -- Vec3Double -- ///////////
struct Vec3DoubleAdapter
{
    using ObjectType = hry::Vec3<double>;

    hry::Vec3<double> get(const scs_value_t& param) const
    {
        if (param.type == SCS_VALUE_TYPE_dvector)
        {
            const auto& vec = param.value_dvector;
            return { vec.x, vec.y, vec.z };
        }
        return {};
    }
};

template<>
struct BasicAdapterCreator<hry::Vec3<double>>
{
    static auto create() { return Vec3DoubleAdapter(); }
};

/////////// -- Euler -- ///////////
struct EulerAdapter
{
    using ObjectType = hry::Euler;

    hry::Euler get(const scs_value_t& param) const
    {
        if (param.type == SCS_VALUE_TYPE_euler)
        {
            const auto& euler = param.value_euler;
            return { euler.heading, euler.pitch, euler.roll };
        }
        return {};
    }
};

template<>
struct BasicAdapterCreator<hry::Euler>
{
    static auto create() { return EulerAdapter(); }
};

/////////// -- PlacementF -- ///////////
struct PlacementFAdapter
{
    using ObjectType = hry::PlacementF;

    hry::PlacementF get(const scs_value_t& param) const
    {
        if (param.type == SCS_VALUE_TYPE_fplacement)
        {
            const auto& pl = param.value_fplacement;
            return { { pl.position.x, pl.position.y, pl.position.z },
                     { pl.orientation.heading, pl.orientation.pitch, pl.orientation.roll } };
        }
        return {};
    }
};

template<>
struct BasicAdapterCreator<hry::PlacementF>
{
    static auto create() { return PlacementFAdapter(); }
};

/////////// -- PlacementF -- ///////////
struct PlacementDAdapter
{
    using ObjectType = hry::PlacementD;

    hry::PlacementD get(const scs_value_t& param) const
    {
        if (param.type == SCS_VALUE_TYPE_dplacement)
        {
            const auto& pl = param.value_dplacement;
            return { { pl.position.x, pl.position.y, pl.position.z },
                     { pl.orientation.heading, pl.orientation.pitch, pl.orientation.roll } };
        }
        return {};
    }
};

template<>
struct BasicAdapterCreator<hry::PlacementD>
{
    static auto create() { return PlacementDAdapter(); }
};

/////////// -- Strign -- ///////////
struct StringAdapter
{
    using ObjectType = std::string;

    std::string get(const scs_value_t& param) const
    {
        if (param.type == SCS_VALUE_TYPE_string)
        {
            return param.value_string.value;
        }
        return {};
    }
};

template<>
struct BasicAdapterCreator<std::string>
{
    static auto create() { return StringAdapter(); }
};

template<typename, typename = std::void_t<>>
struct HasBasicAdapter : std::false_type
{
};

template<typename T>
struct HasBasicAdapter<T, std::void_t<decltype(BasicAdapterCreator<T>::create())>> : std::true_type
{
};

template<typename T>
inline constexpr auto HasAdapter_v = HasBasicAdapter<T>::value;

template<typename T>
auto CreateBasicAdapter()
{
    return BasicAdapterCreator<T>::create();
}
