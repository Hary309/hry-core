#pragma once

#include "Hry/SCSSDK/TrailerChannel.hpp"

#include "Events/EventProxyBase.hpp"
#include "Events/InternalEventHandler.hpp"

#include "ChannelAggregatorBase.hpp"

HRY_NS_BEGIN

class TrailerChannelAggregator : public ChannelAggregatorBase
{
private:
    scs::TrailerChannel& _trailer;

    uint32_t _wheelCount = 0;

public:
    TrailerChannelAggregator(
        scs::TrailerChannel& trailerChannel,
        scs_telemetry_init_params_v100_t* scsTelemetry,
        InternalEventHandler& eventHandler);

private:
    void onTrailerConfig(const std::optional<scs::Trailer>&&);

    void registerWheels(int startIndex, int endIndex);
    void unregisterWheels(int startIndex, int endIndex);
};

HRY_NS_END
