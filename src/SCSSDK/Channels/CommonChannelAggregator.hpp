#pragma once

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/CommonChannel.hpp"

#include "ChannelAggregatorBase.hpp"

HRY_NS_BEGIN

class CommonChannelAggregator : public ChannelAggregatorBase
{
private:
    scs::CommonChannel& _common;

public:
    CommonChannelAggregator(
        scs::CommonChannel& commonChannel, scs_telemetry_init_params_v100_t* scsTelemetry);
};

HRY_NS_END
