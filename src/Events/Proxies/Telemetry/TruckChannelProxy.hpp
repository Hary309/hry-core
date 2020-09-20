#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/ConfigurationData.hpp"
#include "Hry/SCSSDK/TruckChannel.hpp"

#include "Events/EventManager.hpp"

#include "ChannelProxyBase.hpp"

HRY_NS_BEGIN

class TruckChannelProxy : public ChannelProxyBase
{
private:
    scs::TruckChannel _truck{};

    EventManager& _eventMgr;

    Sink<void(const FrameStartEvent&&)> _onFrameStart;
    Sink<void(const std::optional<scs::Truck>&&)> _onTruckConfig;
    Sink<void(const std::optional<scs::HShifter>&&)> _onHShifterConfig;

    uint32_t _selectorCount = 0;
    uint32_t _wheelCount = 0;

public:
    TruckChannelProxy(EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry);

private:
    void frameStart(const FrameStartEvent&&);

    void onTruckConfig(const std::optional<scs::Truck>&& truck);
    void onHShifterConfig(const std::optional<scs::HShifter>&& hshifter);

    void registerWheels(int startIndex, int endIndex);
    void unregisterWheels(int startIndex, int endIndex);
};

HRY_NS_END
