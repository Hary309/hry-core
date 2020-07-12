#include "ImGuiImplEvents.hpp"

#include <imgui.h>

#include "Hry/Events/Event.hpp"

namespace hry
{


ImGuiImplEvents::ImGuiImplEvents(events::EventManager& eventMgr)
    :
        _onMouseButtonPress(eventMgr.mouseButtonPressSignal),
        _onMouseButtonRelease(eventMgr.mouseButtonReleaseSignal),
        _onMouseWheelScroll(eventMgr.mouseWheelScrollSignal)
{
    _onMouseButtonPress.connect<OnMouseButtonPress>();
    _onMouseButtonRelease.connect<OnMouseButtonRelease>();
    _onMouseWheelScroll.connect<OnMouseWheelScroll>();
}

void ImGuiImplEvents::OnMouseButtonPress(const events::MouseButtonEvent&& buttonEvent) 
{
    auto& imguiIO = ImGui::GetIO();
    imguiIO.MouseDown[static_cast<int>(buttonEvent.button)] = true;
}

void ImGuiImplEvents::OnMouseButtonRelease(const events::MouseButtonEvent&& buttonEvent) 
{
    auto& imguiIO = ImGui::GetIO();
    imguiIO.MouseDown[static_cast<int>(buttonEvent.button)] = false;
}

void ImGuiImplEvents::OnMouseWheelScroll(const events::MouseWheelEvent&& wheelEvent) 
{
    auto& imguiIO = ImGui::GetIO();

    if (wheelEvent.wheel == system::Mouse::Wheel::Vertical)
    {
        imguiIO.MouseWheel = static_cast<float>(wheelEvent.delta);
    }
    else
    {
        imguiIO.MouseWheelH = static_cast<float>(wheelEvent.delta);
    }
}

}
