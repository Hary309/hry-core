#include "CommonChannelAggregator.hpp"

#include <common/scssdk_telemetry_common_channels.h>

HRY_NS_BEGIN

CommonChannelAggregator::CommonChannelAggregator(
    scs::CommonChannel& commonChannel, scs_telemetry_init_params_v100_t* scsTelemetry)
    : ChannelAggregatorBase(scsTelemetry), _common(commonChannel)
{
    registerChannel(SCS_TELEMETRY_CHANNEL_local_scale, _common.localScale);
    registerChannel(SCS_TELEMETRY_CHANNEL_game_time, _common.gameTime);
    registerChannel(SCS_TELEMETRY_CHANNEL_next_rest_stop, _common.restStop);
}

HRY_NS_END
