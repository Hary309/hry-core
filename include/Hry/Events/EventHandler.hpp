#pragma once
#include "Event.hpp"
#include "Hry/Utils/Signal.hpp"

namespace hry::events
{

struct EventHandler
{
    utils::Sink<void(const ResizeEvent&&)> onWindowResize;
    utils::Sink<void()> onWindowGainFocus;
    utils::Sink<void()> onWindowLoseFocus;
    
    utils::Sink<void(const KeyboardEvent&&)> onKeyPress;
    utils::Sink<void(const KeyboardEvent&&)> onKeyRelease;

    utils::Sink<void(const MouseButtonEvent&&)> onMouseButtonPress;
    utils::Sink<void(const MouseButtonEvent&&)> onMouseButtonRelease;
    utils::Sink<void(const MouseMoveEvent&&)> onMouseMove;
    utils::Sink<void(const MouseWheelEvent&&)> onMouseWheelScroll;
};

}
