#include "ImGuiImplEvents.hpp"

#include <imgui.h>

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

ImGuiImplEvents::ImGuiImplEvents(EventHandler& eventHandler)
{
    eventHandler.onMouseButtonPress.connect<OnMouseButtonPress>();
    eventHandler.onMouseButtonRelease.connect<OnMouseButtonRelease>();
    eventHandler.onMouseWheelScroll.connect<OnMouseWheelScroll>();
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
