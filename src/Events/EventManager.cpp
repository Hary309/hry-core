#include "EventManager.hpp"

#include "Hry/Namespace.hpp"

#include "Events/Proxies/TelemetryEventProxy.hpp"
#include "Proxies/DInput8EventProxy.hpp"
#include "Proxies/TelemetryEventProxy.hpp"
#include "Proxies/WndProcEventProxy.hpp"

#include "Core.hpp"

HRY_NS_BEGIN

void EventManager::init(scs_telemetry_init_params_v100_t* scsTelemetry)
{
    Core::Logger->info("Initializing EventManager...");

    _eventProxies.push_back(std::make_unique<WndProcEventProxy>(*this));
    _eventProxies.push_back(std::make_unique<DInput8EventProxy>(*this));
    _eventProxies.push_back(std::make_unique<TelemetryEventProxy>(*this, scsTelemetry));
}

EventHandler EventManager::createEventHandler()
{
    // clang-format off
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

        joystickMoveSignal,
        joystickButtonPressSignal,
        joystickButtonReleaseSignal,

        imguiRenderSignal,

        frameStartSignal,
        frameEndSignal,
        stateChangeSignal,
    };
    // clang-format on
}

HRY_NS_END