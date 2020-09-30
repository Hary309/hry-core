/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/ConfigurationData.hpp"
#include "Hry/SCSSDK/TruckChannel.hpp"

#include "Events/EventManager.hpp"

#include "ChannelAggregatorBase.hpp"

HRY_NS_BEGIN

class TruckChannelAggregator : public ChannelAggregatorBase
{
private:
    scs::TruckChannel& _truck;

    uint32_t _selectorCount = 0;
    uint32_t _wheelCount = 0;

public:
    TruckChannelAggregator(
        scs::TruckChannel& truckChannel,
        scs_telemetry_init_params_v100_t* scsTelemetry,
        InternalEventHandler& eventHandler);

private:
    void onTruckConfig(const std::optional<scs::Truck>&& truck);
    void onHShifterConfig(const std::optional<scs::HShifter>&& hshifter);

    void registerWheels(int startIndex, int endIndex);
    void unregisterWheels(int startIndex, int endIndex);
};

HRY_NS_END
