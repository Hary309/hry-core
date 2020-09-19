#pragma once

#include <type_traits>

#include <scssdk_telemetry.h>

#include "Hry/Namespace.hpp"

#include "SCSSDK/Adapters.hpp"
#include "SCSSDK/ValueType.hpp"

HRY_NS_BEGIN

template<typename T>
void storeValue(
    const scs_string_t name,
    const scs_u32_t index,
    const scs_value_t* const value,
    const scs_context_t context)
{
    if (value != nullptr)
    {
        *static_cast<T*>(context) = valueAdapter<T>(*value);
    }
    else
    {
        *static_cast<T*>(context) = T{};
    }
}

class ChannelProxyBase
{
private:
    scs_telemetry_init_params_v100_t* _scsTelemetry;

public:
    ChannelProxyBase(scs_telemetry_init_params_v100_t* scsTelemetry) : _scsTelemetry(scsTelemetry)
    {
    }

    // add support for indexed
    template<typename ValueType>
    void registerChannel(const char* id, ValueType& member)
    {
        _scsTelemetry->register_for_channel(
            id, SCS_U32_NIL, SCSValueType_v<ValueType>, SCS_TELEMETRY_CHANNEL_FLAG_no_value,
            &storeValue<ValueType>, &member);
    }

private:
};

HRY_NS_END
