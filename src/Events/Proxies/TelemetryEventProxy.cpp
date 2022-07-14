/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "TelemetryEventProxy.hpp"

#include <scssdk_telemetry.h>

#include "Hry/Events/Event.hpp"
#include "Hry/Utils/Timer.hpp"

#include "Events/EventManager.hpp"

#include "scssdk.h"

namespace hry
{
TelemetryEventProxy::TelemetryEventProxy(
    EventManager& eventMgr, scs_telemetry_init_params_v100_t* scsTelemetry)
    : EventProxyBase(eventMgr), _configuration(eventMgr, scsTelemetry),
      _gameplay(eventMgr, scsTelemetry)
{
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_frame_start, TelemetryEventProxy::FrameStart, &eventMgr);
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_frame_end, TelemetryEventProxy::FrameEnd, &eventMgr);
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_paused, TelemetryEventProxy::ChangedState, &eventMgr);
    scsTelemetry->register_for_event(
        SCS_TELEMETRY_EVENT_started, TelemetryEventProxy::ChangedState, &eventMgr);
}

void TelemetryEventProxy::FrameStart(
    scs_event_t /*unused*/, const void* data, scs_context_t context) noexcept
{
    static Timer timer;
    static scs_timestamp_t lastSimulationTime;

    const auto& frameStartData = *reinterpret_cast<const scs_telemetry_frame_start_t*>(data);

    auto* eventMgr = reinterpret_cast<EventManager*>(context);
    eventMgr->game.frameStartSignal.call(
        { timer.asSeconds(), frameStartData.paused_simulation_time == lastSimulationTime });
    timer.reset();

    lastSimulationTime = frameStartData.paused_simulation_time;
}

void TelemetryEventProxy::FrameEnd(
    scs_event_t /*unused*/, const void* /*unused*/, scs_context_t context) noexcept
{
    static Timer timer;

    auto* eventMgr = reinterpret_cast<EventManager*>(context);
    eventMgr->game.frameEndSignal.call({ timer.asSeconds() });
    timer.reset();
}

void TelemetryEventProxy::ChangedState(
    scs_event_t event, const void* /*unused*/, scs_context_t context) noexcept
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
    eventMgr->game.stateChangeSignal.call(GameStateEvent{ type });
}
}
