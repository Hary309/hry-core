#include "ChannelAggregator.hpp"

#include "SCSSDK/Channels/CommonChannelAggregator.hpp"
#include "SCSSDK/Channels/JobChannelAggregator.hpp"
#include "SCSSDK/Channels/TrailerChannelAggregator.hpp"
#include "SCSSDK/Channels/TruckChannelAggregator.hpp"

HRY_NS_BEGIN

ChannelAggregator::ChannelAggregator(InternalEventHandler& eventHandler)
    : _eventHandler(eventHandler)
{
    _eventHandler.game.config.substancesSignal.connect<&ChannelAggregator::onSubstances>(this);
}

void ChannelAggregator::init(scs_telemetry_init_params_v100_t* scsTelemetry)
{
    _channelAggregators.push_back(std::unique_ptr<ChannelAggregatorBase>(
        new TruckChannelAggregator(*_telemetry._truck, scsTelemetry, _eventHandler)));
    _channelAggregators.push_back(std::unique_ptr<ChannelAggregatorBase>(
        new TrailerChannelAggregator(_telemetry._trailers, scsTelemetry, _eventHandler)));
    _channelAggregators.push_back(std::unique_ptr<ChannelAggregatorBase>(
        new JobChannelAggregator(*_telemetry._job, scsTelemetry)));
    _channelAggregators.push_back(std::unique_ptr<ChannelAggregatorBase>(
        new CommonChannelAggregator(*_telemetry._common, scsTelemetry)));
}

void ChannelAggregator::onSubstances(const std::optional<scs::Substances>&& substances)
{
    if (substances.has_value())
    {
        _telemetry._substances = substances.value();
    }
    else
    {
        _telemetry._substances.substances.clear();
    }
}

HRY_NS_END
