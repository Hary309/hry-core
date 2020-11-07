/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "EventManager.hpp"

#include "Core.hpp"

#include "Events/Proxies/TelemetryEventProxy.hpp"
#include "Events/Proxies/XInputEventProxy.hpp"
#include "Proxies/DInput8EventProxy.hpp"
#include "Proxies/TelemetryEventProxy.hpp"
#include "Proxies/WndProcEventProxy.hpp"

namespace hry
{
void EventManager::init(scs_telemetry_init_params_v100_t* scsTelemetry)
{
    Core::Logger->info("Initializing EventManager...");

    _eventProxies.push_back(std::make_unique<WndProcEventProxy>(*this));
    _eventProxies.push_back(std::make_unique<DInput8EventProxy>(*this));
    _eventProxies.push_back(std::make_unique<XInputEventProxy>(*this));
    _eventProxies.push_back(std::make_unique<TelemetryEventProxy>(*this, scsTelemetry));
}

EventDispatcher EventManager::createEventDispatcher()
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

InternalEventDispatcher EventManager::createInternalEventDispatcher()
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
            system.overlayStateChangeSignal
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
}