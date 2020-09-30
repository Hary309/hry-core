/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

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
    std::vector<std::unique_ptr<ChannelAggregatorBase>> _channelAggregators;

    Telemetry _telemetry;

    InternalEventHandler& _eventHandler;

public:
    ChannelAggregator(InternalEventHandler& eventHandler);

    void init(scs_telemetry_init_params_v100_t* scsTelemetry);

    const Telemetry& getTelemetry() const { return _telemetry; }

private:
    void onSubstances(const std::optional<scs::Substances>&&);
};

HRY_NS_END
