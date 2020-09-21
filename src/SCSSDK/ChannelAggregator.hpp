#pragma once

#include <memory>
#include <vector>

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/Telemetry.hpp"

#include "Events/InternalEventHandler.hpp"
#include "SCSSDK/Channels/ChannelAggregatorBase.hpp"

HRY_NS_BEGIN

class ChannelAggregatorBase;

class ChannelAggregator
{
private:
    Telemetry _telemetry;

    InternalEventHandler& _eventHandler;

    std::vector<std::unique_ptr<ChannelAggregatorBase>> _channelAggregators;

public:
    ChannelAggregator(InternalEventHandler& eventHandler);

    void init(scs_telemetry_init_params_v100_t* scsTelemetry);
};

HRY_NS_END
