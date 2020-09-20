#pragma once

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/ConfigurationData.hpp"
#include "Hry/SCSSDK/TruckChannel.hpp"

#include "Events/EventManager.hpp"

#include "ChannelProxyBase.hpp"

HRY_NS_BEGIN

class TruckChannelProxy : public ChannelProxyBase
{
private:
    TruckChannel _truck{};

    Sink<void(const std::optional<scs::Truck>&&)> _onTruckConfig;
    Sink<void(const std::optional<scs::HShifter>&&)> _onHShifterConfig;

    uint32_t _selectorCount = 0;
    uint32_t _wheelCount = 0;

public:
    TruckChannelProxy(EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry);
    ~TruckChannelProxy() { Core::Logger->info("Umieram se"); }

    TruckChannelProxy(TruckChannelProxy&&) = delete;
    TruckChannelProxy(const TruckChannelProxy&) = delete;
    TruckChannelProxy& operator=(TruckChannelProxy&&) = delete;
    TruckChannelProxy& operator=(const TruckChannelProxy&) = delete;

private:
    void onTruckConfig(const std::optional<scs::Truck>&& truck);
    void onHShifterConfig(const std::optional<scs::HShifter>&& hshifter);

    void registerWheels(int startIndex, int endIndex);
    void unregisterWheels(int startIndex, int endIndex);
};

HRY_NS_END
