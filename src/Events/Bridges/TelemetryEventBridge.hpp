#pragma once

#include <scssdk_telemetry.h>

#include "Hry/Namespace.hpp"

#include "Events/EventBridgeBase.hpp"

HRY_NS_BEGIN

class TelemetryEventBridge : public EventBridgeBase
{
private:
    scs_telemetry_init_params_v100_t* _scsTelemetry;

public:
    TelemetryEventBridge(EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry);

private:
    static void FrameStart(
        const scs_event_t event, const void* const event_info, const scs_context_t context);
    static void FrameEnd(
        const scs_event_t event, const void* const event_info, const scs_context_t context);
    static void ChangedState(
        const scs_event_t event, const void* const event_info, const scs_context_t context);
};

HRY_NS_END
