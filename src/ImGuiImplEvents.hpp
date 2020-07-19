#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventManager.hpp"

namespace hry
{

// implement missing mouse events for ImGui (thx dinput8)
class ImGuiImplEvents
{
private:
    Sink<void(const MouseButtonEvent&&)> _onMouseButtonPress;
    Sink<void(const MouseButtonEvent&&)> _onMouseButtonRelease;
    Sink<void(const MouseWheelEvent&&)> _onMouseWheelScroll;

public:
    ImGuiImplEvents(EventManager& eventMgr);

private:
    static void OnMouseButtonPress(const MouseButtonEvent&&);
    static void OnMouseButtonRelease(const MouseButtonEvent&&);
    static void OnMouseWheelScroll(const MouseWheelEvent&&);
};

}
