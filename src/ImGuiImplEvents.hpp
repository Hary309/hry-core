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
    utils::Sink<void(const events::MouseButtonEvent&&)> _onMouseButtonPress;
    utils::Sink<void(const events::MouseButtonEvent&&)> _onMouseButtonRelease;
    utils::Sink<void(const events::MouseWheelEvent&&)> _onMouseWheelScroll;

public:
    ImGuiImplEvents(events::EventManager& eventMgr);

private:
    static void OnMouseButtonPress(const events::MouseButtonEvent&&);
    static void OnMouseButtonRelease(const events::MouseButtonEvent&&);
    static void OnMouseWheelScroll(const events::MouseWheelEvent&&);
};

}
