#pragma once
#include "Event.hpp"
#include "Hry/Utils/Signal.hpp"

namespace hry
{
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

    Sink<void()> onImGuiRender;

    // game events
    Sink<void()> onFrameStart;
    Sink<void()> onFrameEnd;
    Sink<void(const GameStateEvent&&)> onStateChange;
};

} // namespace hry
