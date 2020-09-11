#pragma once

#include <scssdk_telemetry.h>

#include "Hry/Namespace.hpp"

#include "Events/EventManager.hpp"

HRY_NS_BEGIN

class TelemetryGameplayProxy
{
private:
public:
    TelemetryGameplayProxy(EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry);

private:
    static void Gameplay(scs_event_t event, const void* event_info, scs_context_t context);
};

HRY_NS_END