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
    Signal<void()> onFrameStart;
    Signal<void()> onFrameEnd;
    Signal<void(const GameStateEvent&&)> onStateChange;
};

}
