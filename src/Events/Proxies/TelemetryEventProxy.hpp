#pragma once

#include <scssdk_telemetry.h>

#include "Hry/Namespace.hpp"

#include "Events/EventProxyBase.hpp"

HRY_NS_BEGIN

class TelemetryEventProxy : public EventProxyBase
{
private:
    scs_telemetry_init_params_v100_t* _scsTelemetry;

public:
    TelemetryEventProxy(EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry);

private:
    static void FrameStart(scs_event_t event, const void* event_info, scs_context_t context);
    static void FrameEnd(scs_event_t event, const void* event_info, scs_context_t context);
    static void ChangedState(scs_event_t event, const void* event_info, scs_context_t context);
};

HRY_NS_END