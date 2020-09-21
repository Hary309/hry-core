#pragma once

#include <optional>
#include <type_traits>

#include <scssdk_telemetry.h>

#include "Hry/Namespace.hpp"

#include "SCSSDK/Adapters.hpp"
#include "SCSSDK/ValueType.hpp"

#include "scssdk.h"

HRY_NS_BEGIN

template<typename T>
inline void storeValue(
    const scs_string_t /*unused*/,
    const scs_u32_t /*unused*/,
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
inline void storeIndexedValue(
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

class ChannelAggregatorBase
{
private:
    scs_telemetry_register_for_channel_t register_for_channel;
    scs_telemetry_unregister_from_channel_t unregister_from_channel;

public:
    ChannelAggregatorBase(scs_telemetry_init_params_v100_t* scsTelemetry);

protected:
    template<typename ValueType>
    void registerChannel(const char* id, ValueType& member)
    {
        auto result = register_for_channel(
            id, SCS_U32_NIL, SCSValueType_v<ValueType>, SCS_TELEMETRY_CHANNEL_FLAG_no_value,
            &storeValue<ValueType>, &member);

        checkForError(id, {}, result);
    }

    template<typename ValueType>
    void registerIndexedChannel(
        const char* id, uint32_t beginIndex, uint32_t count, std::vector<ValueType>& member)
    {
        for (uint32_t i = beginIndex; i < beginIndex + count; i++)
        {
            auto result = register_for_channel(
                id, i, SCSValueType_v<ValueType>, SCS_TELEMETRY_CHANNEL_FLAG_no_value,
                &storeIndexedValue<ValueType>, &member);

            checkForError(id, i, result);
        }
    }

    template<typename ValueType>
    void unregisterIndexedChannel(
        const char* id, uint32_t startIndex, uint32_t endIndex, std::vector<ValueType>& /*unused*/)
    {
        for (uint32_t i = startIndex; i < endIndex; i++)
        {
            auto result = unregister_from_channel(id, i, SCSValueType_v<ValueType>);

            checkForError(id, i, result);
        }
    }

private:
    static void checkForError(const char* id, std::optional<uint32_t> index, scs_result_t result);
};

HRY_NS_END
