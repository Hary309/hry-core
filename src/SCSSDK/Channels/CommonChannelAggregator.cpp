/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "CommonChannelAggregator.hpp"

#include <common/scssdk_telemetry_common_channels.h>

namespace hry
{
CommonChannelAggregator::CommonChannelAggregator(scs::CommonChannel& commonChannel, scs_telemetry_init_params_v100_t* scsTelemetry)
    : ChannelAggregatorBase(scsTelemetry)
    , _common(commonChannel)
{
    registerChannel(SCS_TELEMETRY_CHANNEL_local_scale, _common.localScale);
    registerChannel(SCS_TELEMETRY_CHANNEL_game_time, _common.gameTime);
    registerChannel(SCS_TELEMETRY_CHANNEL_next_rest_stop, _common.restStop);
}
}
