#include "EventManager.hpp"

#include "Core.hpp"

#include "Bridges/WndProcEventBridge.hpp"
#include "Bridges/DInput8EventBridge.hpp"
#include "Bridges/TelemetryEventBridge.hpp"
#include "Events/Bridges/TelemetryEventBridge.hpp"

namespace hry::events
{


void EventManager::init(scs_telemetry_init_params_v100_t* scsTelemetry) 
{
    Core::Logger->info("Initializing EventManager...");

    _eventBridges.push_back(std::make_unique<WndProcEventBridge>(*this));
    _eventBridges.push_back(std::make_unique<DInput8EventBridge>(*this));
    _eventBridges.push_back(std::make_unique<TelemetryEventBridge>(*this, scsTelemetry));
}

EventHandler EventManager::createEventHandler() 
{
    return {
        windowResizeSignal,
        windowGainFocusSignal,
        windowLoseFocusSignal,

        keyPressSignal,
        keyReleaseSignal,

        mouseButtonPressSignal,
        mouseButtonReleaseSignal,
        mouseMoveSignal,
        mouseWheelScrollSignal,

        imguiRenderSignal,

        frameStartSignal,
        frameEndSignal,
        stateChangeSignal
    };
}

}