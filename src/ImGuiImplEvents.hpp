/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventDispatcher.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Events/EventManager.hpp"

HRY_NS_BEGIN

// implement missing mouse events for ImGui (thx dinput8)
class ImGuiImplEvents
{
public:
    ImGuiImplEvents(InternalEventDispatcher& dispatcher);

private:
    static void OnMouseButtonPress(const MouseButtonEvent&&);
    static void OnMouseButtonRelease(const MouseButtonEvent&&);
    static void OnMouseWheelScroll(const MouseWheelEvent&&);
};

HRY_NS_END
