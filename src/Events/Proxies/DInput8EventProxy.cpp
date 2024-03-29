/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "DInput8EventProxy.hpp"

#include <cmath>
#include <cstdio>

#include <dinput.h>

#include "Hry/Events/Event.hpp"
#include "Hry/System/Joystick.hpp"
#include "Hry/System/Mouse.hpp"
#include "Hry/System/System.hpp"

#include "Events/EventManager.hpp"
#include "Hooks/DInput8Hook.hpp"

#undef max

constexpr int DI_MOUSE_X = offsetof(DIMOUSESTATE2, lX);
constexpr int DI_MOUSE_Y = offsetof(DIMOUSESTATE2, lY);
constexpr int DI_MOUSE_Z = offsetof(DIMOUSESTATE2, lZ);
constexpr int DI_MOUSE_BUTTON_0 = (offsetof(DIMOUSESTATE2, rgbButtons) + 0);
constexpr int DI_MOUSE_BUTTON_7 = (offsetof(DIMOUSESTATE2, rgbButtons) + 7);

constexpr int DI_JOYSTICK_X = offsetof(DIJOYSTATE, lX);
// constexpr int DI_JOYSTICK_RX = offsetof(DIJOYSTATE, lRx);
// constexpr int DI_JOYSTICK_RY = offsetof(DIJOYSTATE, lRy);
// constexpr int DI_JOYSTICK_RZ = offsetof(DIJOYSTATE, lRz);
// constexpr int DI_JOYSTICK_SLIDER_0 = (offsetof(DIJOYSTATE, rglSlider) + 0);
// constexpr int DI_JOYSTICK_SLIDER_1 = (offsetof(DIJOYSTATE, rglSlider) + 1);
constexpr int DI_JOYSTICK_POV_0 = (offsetof(DIJOYSTATE, rgdwPOV));
constexpr int DI_JOYSTICK_BUTTON_0 = (offsetof(DIJOYSTATE, rgbButtons) + 0);
constexpr int DI_JOYSTICK_BUTTON_31 = (offsetof(DIJOYSTATE, rgbButtons) + 31);

HRY_NS_BEGIN

DInput8EventProxy::DInput8EventProxy(EventManager& eventMgr) : EventProxyBase(eventMgr)
{
    DInput8Hook::OnMouseData.connect<&DInput8EventProxy::onMouseData>(this);
    DInput8Hook::OnJoystickData.connect<&DInput8EventProxy::onJoystickData>(this);
}

void DInput8EventProxy::onMouseData(const std::vector<DIDEVICEOBJECTDATA>&& events) noexcept
{
    for (const auto& event : events)
    {
        auto offset = event.dwOfs;

        switch (offset)
        {
            // mouse move x
            case DI_MOUSE_X:
            {
                MouseMoveEvent moveEvent{ { static_cast<int>(event.dwData), 0 } };
                _eventMgr.system.mouseMoveSignal.call(std::move(moveEvent));
            }
            break;
            case DI_MOUSE_Y:
            {
                MouseMoveEvent moveEvent{ { 0, static_cast<int>(event.dwData) } };
                _eventMgr.system.mouseMoveSignal.call(std::move(moveEvent));
            }
            break;

            // mouse move wheel
            case DI_MOUSE_Z:
            {
                MouseWheelEvent wheelEvent{};
                wheelEvent.wheel = Mouse::Wheel::Vertical;
                wheelEvent.delta =
                    static_cast<short>(static_cast<short>(event.dwData) / WHEEL_DELTA);

                _eventMgr.system.mouseWheelScrollSignal.call(std::move(wheelEvent));
            }

            default:
            {
                if (offset >= DI_MOUSE_BUTTON_0 && offset <= DI_MOUSE_BUTTON_7)
                {
                    sendMouseButtonEvent(
                        event.dwData, static_cast<Mouse::Button>(offset - DI_MOUSE_BUTTON_0));
                }
            }
        }
    }
}

void DInput8EventProxy::sendMouseButtonEvent(int pressData, Mouse::Button button) noexcept
{
    MouseButtonEvent mouseButtonEvent{};
    mouseButtonEvent.button = button;

    // if pressed
    if (pressData == 0x80)
    {
        mouseButtonEvent.state = ButtonState::Pressed;
        _eventMgr.system.mouseButtonPressSignal.call(std::move(mouseButtonEvent));
    }
    else
    {
        mouseButtonEvent.state = ButtonState::Released;
        _eventMgr.system.mouseButtonReleaseSignal.call(std::move(mouseButtonEvent));
    }
}

void DInput8EventProxy::onJoystickData(
    const std::vector<DIDEVICEOBJECTDATA>&& events, const GUID& guid) noexcept
{
    for (const auto& event : events)
    {
        const auto offset = event.dwOfs;

        if (offset >= DI_JOYSTICK_BUTTON_0 && offset <= DI_JOYSTICK_BUTTON_31)
        {
            auto button = static_cast<Joystick::Button>(offset - DI_JOYSTICK_BUTTON_0);

            if (event.dwData != 0)
            {
                sendJoystickButtonEvent(guid, button, ButtonState::Pressed);
            }
            else
            {
                sendJoystickButtonEvent(guid, button, ButtonState::Released);
            }
        }
        else
        {
            // D-pad
            if (offset == DI_JOYSTICK_POV_0)
            {
                const auto value = LOWORD(event.dwData);

                double x = 0;
                double y = 0;

                if (value != 0xFFFF)
                {
                    auto angle = (static_cast<double>(value)) * M_PI / DI_DEGREES / 180.0;

                    x = std::sin(angle) * 180.0;
                    y = std::cos(angle) * 180.0;
                }

                auto& lastStatus = _dpadStatus[guid];

                sendJoystickDPadEvent(
                    x, guid, lastStatus.right, lastStatus.left, Joystick::Button::DpadRight,
                    Joystick::Button::DpadLeft);

                sendJoystickDPadEvent(
                    y, guid, lastStatus.up, lastStatus.down, Joystick::Button::DpadUp,
                    Joystick::Button::DpadDown);
            }
            else // other analogs
            {
                double result = (static_cast<double>(static_cast<int>(event.dwData)) * 100.0) /
                                static_cast<double>(std::numeric_limits<uint16_t>::max());

                JoystickMoveEvent e{};
                e.deviceGUID = guid;
                e.axis = static_cast<Joystick::Axis>(offset / sizeof(DI_JOYSTICK_X));
                e.value = result;

                _eventMgr.system.joystickMoveSignal.call(std::move(e));
            }
        }
    }
}

void DInput8EventProxy::sendJoystickButtonEvent(
    GUID deviceGUID, Joystick::Button button, ButtonState buttonState)
{
    JoystickButtonEvent e{};
    e.deviceGUID = deviceGUID;
    e.button = button;
    e.state = buttonState;
    _eventMgr.system.joystickButtonPressSignal.call(std::move(e));
}

void DInput8EventProxy::sendJoystickDPadEvent(
    double value,
    GUID deviceGUID,
    bool& field1,
    bool& field2,
    Joystick::Button button1,
    Joystick::Button button2)
{
    // ugh ugly
    // TODO: make it pretty someday
    if (value > 0.1)
    {
        if (!field1)
        {
            sendJoystickButtonEvent(deviceGUID, button1, ButtonState::Pressed);
            field1 = true;
        }

        if (field2)
        {
            sendJoystickButtonEvent(deviceGUID, button2, ButtonState::Released);
            field2 = false;
        }
    }
    else if (value < -0.1)
    {
        if (!field2)
        {
            sendJoystickButtonEvent(deviceGUID, button2, ButtonState::Pressed);
            field2 = true;
        }
        if (field1)
        {
            sendJoystickButtonEvent(deviceGUID, button1, ButtonState::Released);
            field2 = false;
        }
    }
    else
    {
        if (field1)
        {
            sendJoystickButtonEvent(deviceGUID, button1, ButtonState::Released);
            field1 = false;
        }

        if (field2)
        {
            sendJoystickButtonEvent(deviceGUID, button2, ButtonState::Released);
            field2 = false;
        }
    }
}

HRY_NS_END
