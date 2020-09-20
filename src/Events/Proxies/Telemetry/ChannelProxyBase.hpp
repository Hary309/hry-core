#pragma once

#include <type_traits>

#include <scssdk_telemetry.h>

#include "Hry/Namespace.hpp"

#include "SCSSDK/Adapters.hpp"
#include "SCSSDK/ValueType.hpp"

#include "Core.hpp"
#include "scssdk_telemetry_channel.h"

HRY_NS_BEGIN

template<typename T>
void storeValue(
    const scs_string_t, /*unused*/
    const scs_u32_t,
    const scs_value_t* const value,
    const scs_context_t context)
{
    T& ctx = *static_cast<T*>(context);

    if (value != nullptr)
    {
        ctx = valueAdapter<T>(*value);
    }
    else
    {
        ctx = T{};
    }
}

template<typename T>
void storeValueIndexed(
    const scs_string_t /*unused*/,
    const scs_u32_t index,
    const scs_value_t* const value,
    const scs_context_t context)
{
    auto& ctx = *static_cast<std::vector<T>*>(context);

    if (index >= ctx.size())
    {
        ctx.resize(index + 1);
    }

    if (value != nullptr)
    {
        ctx[index] = valueAdapter<T>(*value);
    }
    else
    {
        ctx[index] = T{};
    }
}

class ChannelProxyBase
{
private:
    scs_telemetry_register_for_channel_t register_for_channel;
    scs_telemetry_unregister_from_channel_t unregister_from_channel;

public:
    ChannelProxyBase(scs_telemetry_init_params_v100_t* scsTelemetry)
        : register_for_channel(scsTelemetry->register_for_channel),
          unregister_from_channel(scsTelemetry->unregister_from_channel)
    {
    }

protected:
    // add support for indexed
    template<typename ValueType>
    void registerChannel(const char* id, ValueType& member)
    {
        register_for_channel(
            id, SCS_U32_NIL, SCSValueType_v<ValueType>, SCS_TELEMETRY_CHANNEL_FLAG_no_value,
            &storeValue<ValueType>, &member);
    }

    template<typename ValueType>
    void registerIndexedChannel(
        const char* id, uint32_t beginIndex, uint32_t count, std::vector<ValueType>& member)
    {
        for (uint32_t i = beginIndex; i < beginIndex + count; i++)
        {
            auto err = register_for_channel(
                id, i, SCSValueType_v<ValueType>, SCS_TELEMETRY_CHANNEL_FLAG_no_value,
                &storeValueIndexed<ValueType>, &member);

            if (err != SCS_RESULT_ok)
            {
                Core::Logger->warning("Cannot register {}[] Error id: {}", id, i, err);
            }
        }
    }

    template<typename ValueType>
    void unregisterIndexedChannel(
        const char* id, uint32_t beginIndex, uint32_t count, std::vector<ValueType>& /*unused*/)
    {
        for (uint32_t i = beginIndex; i < beginIndex + count; i++)
        {
            auto err = unregister_from_channel(id, i, SCSValueType_v<ValueType>);

            if (err != SCS_RESULT_ok)
            {
                Core::Logger->warning("Cannot unregister {}[] Error id: {}", id, i, err);
            }
        }
    }
};

HRY_NS_END
