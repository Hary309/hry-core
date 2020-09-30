/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "ImGuiImplEvents.hpp"

#include <imgui.h>

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

ImGuiImplEvents::ImGuiImplEvents(InternalEventHandler& eventHandler)
{
    eventHandler.system.onMouseButtonPress.connect<OnMouseButtonPress>();
    eventHandler.system.onMouseButtonRelease.connect<OnMouseButtonRelease>();
    eventHandler.system.onMouseWheelScroll.connect<OnMouseWheelScroll>();
}

void ImGuiImplEvents::OnMouseButtonPress(const MouseButtonEvent&& buttonEvent)
{
    auto& imguiIO = ImGui::GetIO();
    imguiIO.MouseDown[static_cast<int>(buttonEvent.button)] = true;
}

void ImGuiImplEvents::OnMouseButtonRelease(const MouseButtonEvent&& buttonEvent)
{
    auto& imguiIO = ImGui::GetIO();
    imguiIO.MouseDown[static_cast<int>(buttonEvent.button)] = false;
}

void ImGuiImplEvents::OnMouseWheelScroll(const MouseWheelEvent&& wheelEvent)
{
    auto& imguiIO = ImGui::GetIO();

    if (wheelEvent.wheel == Mouse::Wheel::Vertical)
    {
        imguiIO.MouseWheel = static_cast<float>(wheelEvent.delta);
    }
    else
    {
        imguiIO.MouseWheelH = static_cast<float>(wheelEvent.delta);
    }
}

HRY_NS_END
