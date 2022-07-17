/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "ControlsPage.hpp"

#include "Core.hpp"

#include "Hry/Utils/ImGuiUtils.hpp"
#include "Hry/Utils/Utils.hpp"

#include <imgui.h>

namespace hry
{
bool SliderDouble(const char* label, double* v, double v_min, double v_max)
{
    return ImGui::SliderScalar(label, ImGuiDataType_Double, v, &v_min, &v_max, "%.3f", 0);
}

ControlsPage::ControlsPage(AxisBindsManager& axisBindsMgr, InternalEventDispatcher& dispatcher)
    : _axisBindsMgr(axisBindsMgr)
{
    dispatcher.system.onJoystickMove.connect<&ControlsPage::onJoystickMove>(this);
}

void ControlsPage::imguiRender()
{
    _taskScheduler.update();

    const auto& axisBindsList = _axisBindsMgr.getAxisBinds();

    for (const auto& axisBindsSection : axisBindsList)
    {
        auto& axisBinds = axisBindsSection->getAxisBinds();

        if (axisBinds.empty())
        {
            continue;
        }

        if (ImGui::CollapsingHeader(axisBindsSection->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Columns(4);
            ImGui::SetColumnOffset(3, ImGui::GetWindowContentRegionWidth() - 46);
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

                    if (axisBind->deviceGUID.has_value() && ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text("GUID: %s", fmt::format("{}", axisBind->deviceGUID.value()).c_str());
                        ImGui::EndTooltip();
                    }
                }

                ImGui::NextColumn();

                if (SliderDouble("##drag", &axisBind->deadZone, 0.0, 100.0))
                {
                    axisBindsSection->saveToFile();
                }

                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("Dead zone");
                    ImGui::EndTooltip();
                }

                ImGui::NextColumn();

                if (ImGui::Button("Unset##KeyBinds"))
                {
                    axisBind->axis.reset();
                    axisBind->deviceGUID.reset();
                    axisBindsSection->saveToFile();
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
    if (_axisToSetBind != nullptr && std::abs(e.value) > 25.0)
    {
        if (e.api == JoystickApi::DInput)
        {
            if (!_deviceToBind.has_value())
            {
                _deviceToBind = e;
                _taskScheduler.addTask(
                    std::chrono::milliseconds(250),
                    { ConnectArg_v<&ControlsPage::onBindDInput>, this });
            }
        }
        else if (e.api == JoystickApi::XInput)
        {
            _deviceToBind.reset();
            _axisToSetBind->axis = e.axis;
            _axisToSetBind->deviceGUID = e.deviceGUID;
            _axisToSetBind = nullptr;
            _axisBindsMgr.saveAll();
        }
    }
}

void ControlsPage::onBindDInput()
{
    if (_axisToSetBind != nullptr && _deviceToBind.has_value())
    {
        _axisToSetBind->axis = _deviceToBind->axis;
        _axisToSetBind->deviceGUID = _deviceToBind->deviceGUID;
        _axisToSetBind = nullptr;
        _deviceToBind.reset();
        _axisBindsMgr.saveAll();
    }
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
        default: break;
    }
    return "";
}
}
