/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "XInputEventProxy.hpp"

#include "Core.hpp"
#include "Hooks/XInputHook.hpp"

#include "Hry/Events/Event.hpp"
#include "Hry/System/Joystick.hpp"
#include "Hry/System/System.hpp"

#include <bitset>
#include <limits>

#undef max

namespace hry
{
XInputEventProxy::XInputEventProxy(EventManager& eventMgr)
    : EventProxyBase(eventMgr)
{
    XInputHook::OnJoystickData.connect<&XInputEventProxy::onJoystickData>(this);
}

void XInputEventProxy::onJoystickData(uint32_t index, uint32_t status, XINPUT_STATE& currentState)
{
    // check if device is connected
    if (status != ERROR_SUCCESS)
    {
        return;
    }

    const auto& lastState = _devices[index];

    // nothing changed
    if (currentState.dwPacketNumber <= lastState.dwPacketNumber)
    {
        return;
    }

    const auto& currGP = currentState.Gamepad;
    const auto& lastGP = lastState.Gamepad;

    sendAnalogChange(index, Joystick::Axis::X, currGP, lastGP, &XINPUT_GAMEPAD::bLeftTrigger);
    sendAnalogChange(index, Joystick::Axis::Y, currGP, lastGP, &XINPUT_GAMEPAD::bRightTrigger);
    sendAnalogChange(index, Joystick::Axis::Z, currGP, lastGP, &XINPUT_GAMEPAD::sThumbLX);
    sendAnalogChange(index, Joystick::Axis::R, currGP, lastGP, &XINPUT_GAMEPAD::sThumbLY);
    sendAnalogChange(index, Joystick::Axis::U, currGP, lastGP, &XINPUT_GAMEPAD::sThumbRX);
    sendAnalogChange(index, Joystick::Axis::V, currGP, lastGP, &XINPUT_GAMEPAD::sThumbRY);

    if (currGP.wButtons != lastGP.wButtons)
    {
        std::bitset<16> currBtn{ currGP.wButtons };
        std::bitset<16> lastBtn{ lastGP.wButtons };

        for (size_t i = 0; i < 16; i++)
        {
            bool state = currBtn[i];

            if (state != lastBtn[i])
            {
                JoystickButtonEvent e{};
                e.deviceGUID = getDeviceGUID(index);
                e.button =
                    static_cast<Joystick::Button>(static_cast<int>(Joystick::Button::Button1) + i);
                e.state = state ? ButtonState::Pressed : ButtonState::Released;

                if (state)
                {
                    _eventMgr.system.joystickButtonPressSignal.call(std::move(e));
                }
                else
                {
                    _eventMgr.system.joystickButtonReleaseSignal.call(std::move(e));
                }
            }
        }
    }

    _devices[index] = currentState;
}
}
