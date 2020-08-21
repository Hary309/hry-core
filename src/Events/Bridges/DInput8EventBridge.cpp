#include "DInput8EventBridge.hpp"

#include <cstdio>

#include <dinput.h>

#include "Hry/Events/Event.hpp"
#include "Hry/System/Mouse.hpp"

#include "Events/EventManager.hpp"
#include "Hooks/DInput8Hook.hpp"

constexpr int DI_MOUSE_X = offsetof(DIMOUSESTATE2, lX);
constexpr int DI_MOUSE_Y = offsetof(DIMOUSESTATE2, lY);
constexpr int DI_MOUSE_Z = offsetof(DIMOUSESTATE2, lZ);
constexpr int DI_MOUSE_BUTTON_0 = (offsetof(DIMOUSESTATE2, rgbButtons) + 0);
constexpr int DI_MOUSE_BUTTON_7 = (offsetof(DIMOUSESTATE2, rgbButtons) + 7);

constexpr int DI_JOYSTICK_X = offsetof(DIJOYSTATE, lX);
constexpr int DI_JOYSTICK_Y = offsetof(DIJOYSTATE, lY);
constexpr int DI_JOYSTICK_Z = offsetof(DIJOYSTATE, lZ);
constexpr int DI_JOYSTICK_RX = offsetof(DIJOYSTATE, lRx);
constexpr int DI_JOYSTICK_RY = offsetof(DIJOYSTATE, lRy);
constexpr int DI_JOYSTICK_RZ = offsetof(DIJOYSTATE, lRz);
constexpr int DI_JOYSTICK_SLIDER_0 = (offsetof(DIJOYSTATE, rglSlider) + 0);
constexpr int DI_JOYSTICK_SLIDER_1 = (offsetof(DIJOYSTATE, rglSlider) + 1);
constexpr int DI_JOYSTICK_POV_0 = (offsetof(DIJOYSTATE, rgdwPOV) + 0);
constexpr int DI_JOYSTICK_POV_1 = (offsetof(DIJOYSTATE, rgdwPOV) + 1);
constexpr int DI_JOYSTICK_BUTTON_0 = (offsetof(DIJOYSTATE, rgbButtons) + 0);
constexpr int DI_JOYSTICK_BUTTON_31 = (offsetof(DIJOYSTATE, rgbButtons) + 31);

HRY_NS_BEGIN

DInput8EventBridge::DInput8EventBridge(EventManager& eventMgr) : EventBridgeBase(eventMgr)
{
    DInput8Hook::OnMouseData.connect<&DInput8EventBridge::onMouseData>(this);
    DInput8Hook::OnJoystickData.connect<&DInput8EventBridge::onJoystickData>(this);
}

void DInput8EventBridge::onMouseData(const std::vector<DIDEVICEOBJECTDATA>&& datas)
{
    for (const auto& data : datas)
    {
        auto offset = data.dwOfs;

        switch (offset)
        {
            // mouse move x
            case DI_MOUSE_X:
            {
                _mouseOffset.x = data.dwData;

                MouseMoveEvent moveEvent{ _mouseOffset };
                _eventMgr.mouseMoveSignal.call(std::move(moveEvent));
            }
            break;
            case DI_MOUSE_Y:
            {
                _mouseOffset.y = data.dwData;

                MouseMoveEvent moveEvent{ _mouseOffset };
                _eventMgr.mouseMoveSignal.call(std::move(moveEvent));
            }
            break;

            // mouse move wheel
            case DI_MOUSE_Z:
            {
                MouseWheelEvent wheelEvent{};
                wheelEvent.wheel = Mouse::Wheel::Vertical;
                wheelEvent.delta =
                    static_cast<short>(static_cast<short>(data.dwData) / WHEEL_DELTA);

                _eventMgr.mouseWheelScrollSignal.call(std::move(wheelEvent));
            }

            default:
            {
                if (offset >= DI_MOUSE_BUTTON_0 && offset <= DI_MOUSE_BUTTON_7)
                {
                    sendMouseButtonEvent(
                        data.dwData, static_cast<Mouse::Button>(offset - DI_MOUSE_BUTTON_0));
                }
            }
        }
    }
}

void DInput8EventBridge::sendMouseButtonEvent(int pressData, Mouse::Button button)
{
    MouseButtonEvent mouseButtonEvent{};
    mouseButtonEvent.button = button;

    // if pressed
    if (pressData == 0x80)
    {
        mouseButtonEvent.state = ButtonState::Pressed;
        _eventMgr.mouseButtonPressSignal.call(std::move(mouseButtonEvent));
    }
    else
    {
        mouseButtonEvent.state = ButtonState::Released;
        _eventMgr.mouseButtonReleaseSignal.call(std::move(mouseButtonEvent));
    }
}

void DInput8EventBridge::onJoystickData(
    const std::vector<DIDEVICEOBJECTDATA>&& datas, const DeviceGUID&& guid)
{
    for (const auto& data : datas)
    {
        auto offset = data.dwOfs;

        switch (offset)
        {
        case DI_JOYSTICK_X:
        {
        } break;
        case DI_JOYSTICK_Y:
        {
        } break;
        case DI_JOYSTICK_Z:
        {
        } break;
        case DI_JOYSTICK_RX:
        {
        } break;
        case DI_JOYSTICK_RY:
        {
        } break;
        case DI_JOYSTICK_RZ:
        {
        } break;
        case DI_JOYSTICK_SLIDER_0:
        {
        } break;
        case DI_JOYSTICK_SLIDER_1:
        {
        } break;
        case DI_JOYSTICK_POV_0:
        {
        } break;
        case DI_JOYSTICK_POV_1:
        {
        } break;
        default:
        {
            if (offset >= DI_JOYSTICK_BUTTON_0 && offset <= DI_JOYSTICK_BUTTON_31)
            {
                // TODO: send event
            }
        }
        }
    }
}

HRY_NS_END
