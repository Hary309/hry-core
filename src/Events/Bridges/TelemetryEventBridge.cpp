#include "TelemetryEventBridge.hpp"

#include "Events/EventManager.hpp"
#include "Hry/Events/Event.hpp"
#include "scssdk_telemetry_event.h"

namespace hry
{

TelemetryEventBridge::TelemetryEventBridge(EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry) 
    : EventBridgeBase(eventMgr), _scsTelemetry(scsTelemetry)
{
    _scsTelemetry->register_for_event(SCS_TELEMETRY_EVENT_frame_start, TelemetryEventBridge::FrameStart, &eventMgr);
    _scsTelemetry->register_for_event(SCS_TELEMETRY_EVENT_frame_end, TelemetryEventBridge::FrameEnd, &eventMgr);
    _scsTelemetry->register_for_event(SCS_TELEMETRY_EVENT_paused, TelemetryEventBridge::ChangedState, &eventMgr);
    _scsTelemetry->register_for_event(SCS_TELEMETRY_EVENT_started, TelemetryEventBridge::ChangedState, &eventMgr);

    // TODO: Add SCS_TELEMETRY_EVENT_configuration and SCS_TELEMETRY_EVENT_gameplay
}

void TelemetryEventBridge::FrameStart(const scs_event_t event, const void *const event_info, const scs_context_t context) 
{
    EventManager* eventMgr = reinterpret_cast<EventManager*>(context);
    eventMgr->frameStartSignal.call();
}

void TelemetryEventBridge::FrameEnd(const scs_event_t event, const void *const event_info, const scs_context_t context) 
{
    EventManager* eventMgr = reinterpret_cast<EventManager*>(context);
    eventMgr->frameEndSignal.call();
}

void TelemetryEventBridge::ChangedState(const scs_event_t event, const void *const event_info, const scs_context_t context) 
{
    EventManager* eventMgr = reinterpret_cast<EventManager*>(context);

    GameStateEvent::Type type;

    if (event == SCS_TELEMETRY_EVENT_paused)
    {
        type = GameStateEvent::Type::Paused;
    }
    else
    {
        type = GameStateEvent::Type::Resumed;   
    }
    eventMgr->stateChangeSignal.call(GameStateEvent{type});
}

}
