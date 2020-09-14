#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventManager.hpp"

HRY_NS_BEGIN

// implement missing mouse events for ImGui (thx dinput8)
class ImGuiImplEvents
{
public:
    ImGuiImplEvents(InternalEventHandler& eventHandler);

private:
    static void OnMouseButtonPress(const MouseButtonEvent&&);
    static void OnMouseButtonRelease(const MouseButtonEvent&&);
    static void OnMouseWheelScroll(const MouseWheelEvent&&);
};

HRY_NS_END
