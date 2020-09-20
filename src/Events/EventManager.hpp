#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Windows.h>

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/Logger/Logger.hpp"
#include "Hry/SCSSDK/ConfigurationData.hpp"
#include "Hry/SCSSDK/GameplayData.hpp"
#include "Hry/SCSSDK/TruckChannel.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventProxyBase.hpp"

#include "InternalEventHandler.hpp"

struct scs_telemetry_init_params_v100_t;

HRY_NS_BEGIN

class EventProxyBase;

class EventManager
{
private:
    std::vector<std::unique_ptr<EventProxyBase>> _eventProxies;

public:
    struct
    {
        // internal signals
        Signal<void(std::string msg, Logger::Level)> logSignal;
        Signal<void(HWND, UINT, WPARAM, LPARAM)> wndProcSignal;

        // system events
        Signal<void(const ResizeEvent&&)> windowResizeSignal;
        Signal<void()> windowGainFocusSignal;
        Signal<void()> windowLoseFocusSignal;

        Signal<void(const KeyboardEvent&&)> keyPressSignal;
        Signal<void(const KeyboardEvent&&)> keyReleaseSignal;

        Signal<void(const MouseButtonEvent&&)> mouseButtonPressSignal;
        Signal<void(const MouseButtonEvent&&)> mouseButtonReleaseSignal;
        Signal<void(const MouseMoveEvent&&)> mouseMoveSignal;
        Signal<void(const MouseWheelEvent&&)> mouseWheelScrollSignal;

        Signal<void(const JoystickMoveEvent&&)> joystickMoveSignal;
        Signal<void(const JoystickButtonEvent&&)> joystickButtonPressSignal;
        Signal<void(const JoystickButtonEvent&&)> joystickButtonReleaseSignal;

        Signal<void()> imguiRenderSignal;
    } system;

    // game events
    struct
    {
        Signal<void(const FrameStartEvent&&)> frameStartSignal;
        Signal<void(const FrameEndEvent&&)> frameEndSignal;
        Signal<void(const GameStateEvent&&)> stateChangeSignal;
        Signal<void(const scs::TruckChannel&)> truckChannelSignal;

        // gameplay events
        struct
        {
            Signal<void(const std::optional<scs::JobCancelled>&&)> jobCancelledSignal;
            Signal<void(const std::optional<scs::JobDelivered>&&)> jobDeliveredSignal;
            Signal<void(const std::optional<scs::PlayerFined>&&)> playerFinedSignal;
            Signal<void(const std::optional<scs::PlayerTollgatePaid>&&)> playerTollgatePaidSignal;
            Signal<void(const std::optional<scs::PlayerUseFerry>&&)> playerUseFerrySignal;
            Signal<void(const std::optional<scs::PlayerUseTrain>&&)> playerUseTrainSignal;
        } gameplay;

        // configuration callbacks
        struct
        {
            Signal<void(const std::optional<scs::Substances>&&)> substancesSignal;
            Signal<void(const std::optional<scs::Controls>&&)> controlsSignal;
            Signal<void(const std::optional<scs::HShifter>&&)> hshifterSignal;
            Signal<void(const std::optional<scs::Truck>&&)> truckSignal;
            Signal<void(const std::optional<scs::Trailer>&&)> trailerSignal;
            Signal<void(const std::optional<scs::Job>&&)> jobSignal;
        } config;
    } game;

public:
    void init(scs_telemetry_init_params_v100_t* scsTelemetry);

    [[nodiscard]] EventHandler createEventHandler();
    [[nodiscard]] InternalEventHandler createInternalEventHandler();
};

HRY_NS_END
