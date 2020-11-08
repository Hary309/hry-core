/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "ControlsPage.hpp"

#include <imgui.h>

#include "Hry/Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

ControlsPage::ControlsPage(AxisBindsManager& axisBindsMgr, InternalEventDispatcher& dispatcher)
    : _axisBindsMgr(axisBindsMgr)
{
    dispatcher.system.onJoystickMove.connect<&ControlsPage::onJoystickMove>(this);
}

void ControlsPage::imguiRender()
{
    const auto& axisBindsList = _axisBindsMgr.getAxisBinds();

    for (const auto& axisBindsSection : axisBindsList)
    {
        auto& axisBinds = axisBindsSection->getAxisBinds();

        if (axisBinds.empty())
        {
            continue;
        }

        if (ImGui::CollapsingHeader(
                axisBindsSection->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Columns(3);
            ImGui::SetColumnOffset(2, ImGui::GetWindowContentRegionWidth() - 96);
            for (auto& axisBind : axisBinds)
            {
                ImGui::PushID(&axisBind);

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);

                ImGui::Text("%s", axisBind->label.c_str());

                if (!axisBind->desc.empty())
                {
                    ImGui::SameLine();
                    ImGuiHelpMarker(axisBind->desc.c_str());
                }

                ImGui::NextColumn();

                if (_axisToSetBind == axisBind.get())
                {
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
                    ImGui::Text("Move analog to bind");
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
                }
                else
                {
                    std::string_view text = "Not set";

                    if (axisBind->axis.has_value())
                    {
                        text = GetAxisName(axisBind->axis.value());
                    }

                    if (ImGui::Button(text.data()))
                    {
                        _axisToSetBind = axisBind.get();
                    }
                }

                ImGui::NextColumn();

                if (ImGui::Button("Unset##KeyBinds"))
                {
                    axisBind->axis.reset();
                    axisBind->deviceGUID.reset();
                }

                ImGui::PopID();
                ImGui::NextColumn();
            }
        }

        ImGui::Columns(1);
    }
}

void ControlsPage::onJoystickMove(const JoystickMoveEvent&& e)
{
    auto& curr = _axisValues[static_cast<int>(e.axis)];

    if (_axisToSetBind != nullptr && std::abs(curr - e.value) > 25.0)
    {
        _axisToSetBind->axis = e.axis;
        _axisToSetBind->deviceGUID = e.deviceGUID;
        _axisToSetBind = nullptr;
    }

    curr = e.value;
}

std::string_view ControlsPage::GetAxisName(Joystick::Axis axis)
{
    switch (axis)
    {
        case Joystick::Axis::X: return "Axis X";
        case Joystick::Axis::Y: return "Axis Y";
        case Joystick::Axis::Z: return "Axis Z";
        case Joystick::Axis::R: return "Axis R";
        case Joystick::Axis::U: return "Axis U";
        case Joystick::Axis::V: return "Axis V";
        case Joystick::Axis::PovX: return "Axis Pov X";
        case Joystick::Axis::PovY: return "Axis Pov Y";
        default: break;
    }
    return "";
}

HRY_NS_END
