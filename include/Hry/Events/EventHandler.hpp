#pragma once
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Event.hpp"

HRY_NS_BEGIN

struct EventHandler
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
    } system;

    // game events
    struct
    {
        Sink<void(const FrameEvent&&)> onFrameStart;
        Sink<void(const FrameEvent&&)> onFrameEnd;
        Sink<void(const GameStateEvent&&)> onStateChange;
    } game;
};

HRY_NS_END
