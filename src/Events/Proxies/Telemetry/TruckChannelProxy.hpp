#pragma once

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/TruckChannel.hpp"

#include "Events/EventManager.hpp"

#include "ChannelProxyBase.hpp"

HRY_NS_BEGIN

class TruckChannelProxy : public ChannelProxyBase
{
private:
    TruckChannel _truck;

public:
    TruckChannelProxy(EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry);
};

HRY_NS_END
