/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventDispatcher.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventManager.hpp"

namespace hry
{
// implement missing mouse events for ImGui (thx dinput8)
class ImGuiImplEvents
{
public:
    ImGuiImplEvents(InternalEventDispatcher& dispatcher);

private:
    static void OnOverlayStateChange(const OverlayStateEvent&&);
    static void OnMouseMove(const MouseMoveEvent&&);
    static void OnMouseButtonPress(const MouseButtonEvent&&);
    static void OnMouseButtonRelease(const MouseButtonEvent&&);
    static void OnMouseWheelScroll(const MouseWheelEvent&&);
};
}
