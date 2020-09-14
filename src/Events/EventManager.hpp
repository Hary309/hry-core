#pragma once

#include <memory>
#include <string>
#include <vector>

#include <Windows.h>

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/Logger/Logger.hpp"
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

    // game events
    Signal<void()> frameStartSignal;
    Signal<void()> frameEndSignal;
    Signal<void(const GameStateEvent&&)> stateChangeSignal;

public:
    void init(scs_telemetry_init_params_v100_t* scsTelemetry);

    [[nodiscard]] EventHandler createEventHandler();
    [[nodiscard]] InternalEventHandler createInternalEventHandler();
};

HRY_NS_END
