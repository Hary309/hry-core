#pragma once
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Event.hpp"

HRY_NS_BEGIN

struct EventHandler
{
    // system events
    Sink<void(const ResizeEvent&&)> onWindowResize;
    Sink<void()> onWindowGainFocus;
    Sink<void()> onWindowLoseFocus;

    Sink<void(const KeyboardEvent&&)> onKeyPress;
    Sink<void(const KeyboardEvent&&)> onKeyRelease;

    Sink<void(const MouseButtonEvent&&)> onMouseButtonPress;
    Sink<void(const MouseButtonEvent&&)> onMouseButtonRelease;
    Sink<void(const MouseMoveEvent&&)> onMouseMove;
    Sink<void(const MouseWheelEvent&&)> onMouseWheelScroll;

    Sink<void(const JoystickStatusChangeEvent&&)> onJoystickStatusChange;
    Sink<void(const JoystickMoveEvent&&)> onJoystickMove;
    Sink<void(const JoystickButtonEvent&&)> onJoystickButtonPress;
    Sink<void(const JoystickButtonEvent&&)> onJoystickButtonRelease;

    Sink<void()> onImGuiRender;

    // game events
    Sink<void()> onFrameStart;
    Sink<void()> onFrameEnd;
    Sink<void(const GameStateEvent&&)> onStateChange;
};

HRY_NS_END
