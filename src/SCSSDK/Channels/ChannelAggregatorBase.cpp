/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "ChannelAggregatorBase.hpp"

#include <limits>


#include "Core.hpp"
#include "scssdk.h"

#undef max

namespace hry
{
ChannelAggregatorBase::ChannelAggregatorBase(scs_telemetry_init_params_v100_t* scsTelemetry)
    : register_for_channel(scsTelemetry->register_for_channel),
      unregister_from_channel(scsTelemetry->unregister_from_channel)
{
}

void ChannelAggregatorBase::CheckForError(
    const char* id, std::optional<uint32_t> index, scs_result_t result)
{
    if (result != SCS_RESULT_ok)
    {
        if (index.has_value())
        {
            Core::Logger->warning(
                "Cannot unregister {}[{}] Error id: {}", id, index.value(), result);
        }
        else
        {
            Core::Logger->warning("Cannot unregister {} Error id: {}", id, result);
        }
    }
}
}
