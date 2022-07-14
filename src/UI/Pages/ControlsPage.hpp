/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "AxisBinding/AxisBindsManager.hpp"
#include "Events/InternalEventDispatcher.hpp"
#include "UI/Pages/PageBase.hpp"

#include "Hry/System/Joystick.hpp"

#include <array>

namespace hry
{
class ControlsPage : public PageBase
{
public:
    static constexpr auto AxisCount = 8;

private:
    AxisBindsManager& _axisBindsMgr;

    std::array<double, 8> _axisValues{};
    AxisBind* _axisToSetBind = nullptr;

public:
    explicit ControlsPage(AxisBindsManager& axisBindsMgr, InternalEventDispatcher& dispatcher);

    void imguiRender() override;

private:
    void onJoystickMove(const JoystickMoveEvent&& e);

    static std::string_view GetAxisName(Joystick::Axis axis);
};
}
