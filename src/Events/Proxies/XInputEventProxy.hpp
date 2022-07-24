/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Events/EventManager.hpp"
#include "Events/EventProxyBase.hpp"

#include "Hry/Events/Event.hpp"
#include "Hry/System/Joystick.hpp"

#include <Windows.h>
#include <Xinput.h>
#include <stdint.h>

#include <array>

#undef max

namespace hry
{
class XInputEventProxy : public EventProxyBase
{
private:
    std::array<XINPUT_STATE, 4> _devices{};

public:
    explicit XInputEventProxy(EventManager& eventMgr);

private:
    void onJoystickData(uint32_t index, uint32_t status, XINPUT_STATE& state);

    template<typename ValueType>
    void sendAnalogChange(
        GUID deviceGuid,
        Joystick::Axis axis,
        const XINPUT_GAMEPAD& currState,
        const XINPUT_GAMEPAD& lastState,
        ValueType XINPUT_GAMEPAD::*offset);

    GUID getDeviceGUID(uint32_t index) const;
};
}