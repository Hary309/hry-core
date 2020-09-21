#pragma once

#include <Windows.h>

#include "Hry/Events/Event.hpp"
#include "Hry/Logger/Logger.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/ConfigurationData.hpp"
#include "Hry/Utils/Signal.hpp"

HRY_NS_BEGIN

struct InternalEventHandler
{
    // system events
    struct
    {
        Sink<void(const ResizeEvent&&)> onWindowResize;
        Sink<void()> onWindowGainFocus;
        Sink<void()> onWindowLoseFocus;

        Sink<void(const KeyboardEvent&&)> onKeyPress;
        Sink<void(const KeyboardEvent&&)> onKeyRelease;

        Sink<void(const MouseButtonEvent&&)> onMouseButtonPress;
        Sink<void(const MouseButtonEvent&&)> onMouseButtonRelease;
        Sink<void(const MouseMoveEvent&&)> onMouseMove;
        Sink<void(const MouseWheelEvent&&)> onMouseWheelScroll;

        Sink<void(const JoystickMoveEvent&&)> onJoystickMove;
        Sink<void(const JoystickButtonEvent&&)> onJoystickButtonPress;
        Sink<void(const JoystickButtonEvent&&)> onJoystickButtonRelease;

        Sink<void()> onImGuiRender;

        Sink<void(std::string msg, Logger::Level)> onLog;
        Sink<void(HWND, UINT, WPARAM, LPARAM)> onWndProc;
    } system;

    // game events
    struct
    {
        Sink<void(const FrameStartEvent&&)> onFrameStart;
        Sink<void(const FrameEndEvent&&)> onFrameEnd;
        Sink<void(const GameStateEvent&&)> onStateChange;

        // configuration callbacks
        struct
        {
            Sink<void(const std::optional<scs::Substances>&&)> substancesSignal;
            Sink<void(const std::optional<scs::Controls>&&)> controlsSignal;
            Sink<void(const std::optional<scs::HShifter>&&)> hshifterSignal;
            Sink<void(const std::optional<scs::Truck>&&)> truckSignal;
            Sink<void(const std::optional<scs::Trailer>&&)> trailerSignal;
            Sink<void(const std::optional<scs::Job>&&)> jobSignal;
        } config;
    } game;
};

HRY_NS_END
