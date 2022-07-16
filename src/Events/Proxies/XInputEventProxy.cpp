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

namespace
{
struct XINPUT_CAPABILITIES_EX
{
    XINPUT_CAPABILITIES Capabilities;
    WORD vendorId;
    WORD productId;
    WORD revisionId;
    DWORD a4; //unknown
};

using _XInputGetCapabilitiesEx = DWORD(_stdcall*)(DWORD a1, DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES_EX* pCapabilities);
_XInputGetCapabilitiesEx XInputGetCapabilitiesEx;
}

namespace hry
{
XInputEventProxy::XInputEventProxy(EventManager& eventMgr)
    : EventProxyBase(eventMgr)
{
    HMODULE moduleHandle = LoadLibrary(TEXT("XInput1_4.dll"));
    XInputGetCapabilitiesEx = (_XInputGetCapabilitiesEx)GetProcAddress(moduleHandle, (char*)108);

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

    GUID deviceGuid = getDeviceGUID(index);

    const auto& currGP = currentState.Gamepad;
    const auto& lastGP = lastState.Gamepad;

    sendAnalogChange(deviceGuid, Joystick::Axis::X, currGP, lastGP, &XINPUT_GAMEPAD::bLeftTrigger);
    sendAnalogChange(deviceGuid, Joystick::Axis::Y, currGP, lastGP, &XINPUT_GAMEPAD::bRightTrigger);
    sendAnalogChange(deviceGuid, Joystick::Axis::Z, currGP, lastGP, &XINPUT_GAMEPAD::sThumbLX);
    sendAnalogChange(deviceGuid, Joystick::Axis::R, currGP, lastGP, &XINPUT_GAMEPAD::sThumbLY);
    sendAnalogChange(deviceGuid, Joystick::Axis::U, currGP, lastGP, &XINPUT_GAMEPAD::sThumbRX);
    sendAnalogChange(deviceGuid, Joystick::Axis::V, currGP, lastGP, &XINPUT_GAMEPAD::sThumbRY);

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

template<typename ValueType>
void XInputEventProxy::sendAnalogChange(
    GUID deviceGuid,
    Joystick::Axis axis,
    const XINPUT_GAMEPAD& currState,
    const XINPUT_GAMEPAD& lastState,
    ValueType XINPUT_GAMEPAD::*offset)
{
    if (currState.*offset != lastState.*offset)
    {
        JoystickMoveEvent e{};
        e.deviceGUID = deviceGuid;
        e.axis = axis;
        e.value = static_cast<double>(currState.*offset) /
                  static_cast<double>(std::numeric_limits<ValueType>::max()) * 100.0;

        _eventMgr.system.joystickMoveSignal.call(std::move(e));
    }
}

GUID XInputEventProxy::getDeviceGUID(uint32_t index) const
{
    XINPUT_CAPABILITIES_EX capsEx{};
    if (XInputGetCapabilitiesEx(1, index, 0, &capsEx) == ERROR_SUCCESS)
    {
        GUID guid{};
        guid.Data1 = capsEx.vendorId;
        guid.Data2 = capsEx.productId;
        guid.Data3 = capsEx.revisionId;
        return guid;
    }

    return {};
}
}
