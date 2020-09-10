#include "TelemetryEventProxy.hpp"

#include <scssdk_telemetry.h>

#include "Hry/Events/Event.hpp"
#include "Hry/Namespace.hpp"

#include "Events/EventManager.hpp"

HRY_NS_BEGIN

TelemetryEventProxy::TelemetryEventProxy(
    EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry)
    : EventProxyBase(eventMgr)
{
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_frame_start, TelemetryEventProxy::FrameStart, &eventMgr);
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_frame_end, TelemetryEventProxy::FrameEnd, &eventMgr);
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_paused, TelemetryEventProxy::ChangedState, &eventMgr);
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_started, TelemetryEventProxy::ChangedState, &eventMgr);

    // TODO: Add SCS_TELEMETRY_EVENT_configuration and SCS_TELEMETRY_EVENT_gameplay
}

void TelemetryEventProxy::FrameStart(
    scs_event_t /*unused*/, const void* /*unused*/, scs_context_t context)
{
    auto* eventMgr = reinterpret_cast<EventManager*>(context);
    eventMgr->frameStartSignal.call();
}

void TelemetryEventProxy::FrameEnd(
    scs_event_t /*unused*/, const void* /*unused*/, scs_context_t context)
{
    auto* eventMgr = reinterpret_cast<EventManager*>(context);
    eventMgr->frameEndSignal.call();
}

void TelemetryEventProxy::ChangedState(
    scs_event_t event, const void* /*unused*/, scs_context_t context)
{
    auto* eventMgr = reinterpret_cast<EventManager*>(context);

    GameStateEvent::Type type;

    if (event == SCS_TELEMETRY_EVENT_paused)
    {
        type = GameStateEvent::Type::Paused;
    }
    else
    {
        type = GameStateEvent::Type::Resumed;
    }
    eventMgr->stateChangeSignal.call(GameStateEvent{ type });
}

HRY_NS_END
