#include "ValueType.hpp"

#include "Hry/Math/SCSTypes.hpp"
#include "Hry/Namespace.hpp"

#include "scssdk_value.h"

HRY_NS_BEGIN

namespace scs
{
    Vec3<float> GetVec3f32(const scs_value_fvector_t& value)
    {
        return Vec3<float>{ value.x, value.y, value.z };
    }

    Vec3<double> GetVec3f64(const scs_value_dvector_t& value)
    {
        return Vec3<double>{ value.x, value.y, value.z };
    }

    Euler GetEuler(const scs_value_euler_t& value)
    {
        return Euler{ value.heading, value.pitch, value.roll };
    }

    PlacementF GetPlacementF(const scs_value_fplacement_t& value)
    {
        return PlacementF{ GetVec3f32(value.position), GetEuler(value.orientation) };
    }

    PlacementD GetPlacementD(const scs_value_dplacement_t& value)
    {
        return PlacementD{ GetVec3f64(value.position), GetEuler(value.orientation) };
    }

    ValueType ValueType::Get(const scs_value_t& value)
    {
        switch (value.type)
        {
            case SCS_VALUE_TYPE_bool: return { value.value_bool.value };
            case SCS_VALUE_TYPE_s32: return { value.value_s32.value };
            case SCS_VALUE_TYPE_u32: return { value.value_u32.value };
            case SCS_VALUE_TYPE_u64: return { value.value_u64.value };
            case SCS_VALUE_TYPE_float: return { value.value_float.value };
            case SCS_VALUE_TYPE_double: return { value.value_double.value };
            case SCS_VALUE_TYPE_fvector: return { GetVec3f32(value.value_fvector) };
            case SCS_VALUE_TYPE_dvector: return { GetVec3f64(value.value_dvector) };
            case SCS_VALUE_TYPE_euler: return { GetEuler(value.value_euler) };
            case SCS_VALUE_TYPE_fplacement: return { GetPlacementF(value.value_fplacement) };
            case SCS_VALUE_TYPE_dplacement: return { GetPlacementD(value.value_dplacement) };
            case SCS_VALUE_TYPE_string: return { value.value_string.value };
            case SCS_VALUE_TYPE_s64: return { value.value_s64.value };
            default: return { std::monostate() };
        }
    }
} // namespace scs
HRY_NS_BEGIN
