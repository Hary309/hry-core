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
        {
            system.windowResizeSignal,
            system.windowGainFocusSignal,
            system.windowLoseFocusSignal,

            system.keyPressSignal,
            system.keyReleaseSignal,

            system.mouseButtonPressSignal,
            system.mouseButtonReleaseSignal,
            system.mouseMoveSignal,
            system.mouseWheelScrollSignal,

            system.joystickMoveSignal,
            system.joystickButtonPressSignal,
            system.joystickButtonReleaseSignal,

            system.imguiRenderSignal,
            system.overlayStateChangeSignal
        },
        {
            game.frameStartSignal,
            game.frameEndSignal,
            game.stateChangeSignal,
            {
                game.gameplay.jobCancelledSignal,
                game.gameplay.jobDeliveredSignal,
                game.gameplay.playerFinedSignal,
                game.gameplay.playerTollgatePaidSignal,
                game.gameplay.playerUseFerrySignal,
                game.gameplay.playerUseTrainSignal
            },
            {
                game.config.substancesSignal,
                game.config.controlsSignal,
                game.config.hshifterSignal,
                game.config.truckSignal,
                game.config.trailerSignal,
                game.config.jobSignal
            }
        }
    };
    // clang-format on
}

InternalEventHandler EventManager::createInternalEventHandler()
{
    // clang-format off
    return {
        {
            system.windowResizeSignal,
            system.windowGainFocusSignal,
            system.windowLoseFocusSignal,

            system.keyPressSignal,
            system.keyReleaseSignal,

            system.mouseButtonPressSignal,
            system.mouseButtonReleaseSignal,
            system.mouseMoveSignal,
            system.mouseWheelScrollSignal,

            system.joystickMoveSignal,
            system.joystickButtonPressSignal,
            system.joystickButtonReleaseSignal,

            system.imguiRenderSignal,

            system.logSignal,
            system.wndProcSignal,
        },
        {
            game.frameStartSignal,
            game.frameEndSignal,
            game.stateChangeSignal,
            {
                game.config.substancesSignal,
                game.config.controlsSignal,
                game.config.hshifterSignal,
                game.config.truckSignal,
                game.config.trailerSignal,
                game.config.jobSignal
            }
        }
    };
    // clang-format on
}

HRY_NS_END