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

HRY_NS_BEGIN

DInput8EventBridge::DInput8EventBridge(EventManager& eventMgr) : EventBridgeBase(eventMgr)
{
    DInput8Hook::OnMouseData.connect<&DInput8EventBridge::onMouseData>(this);
    DInput8Hook::OnControllerData.connect<&DInput8EventBridge::onControllerData>(this);
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
                    sendMouseButtonEvent(data.dwData, static_cast<Mouse::Button>(offset - DI_MOUSE_BUTTON_0));
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

void DInput8EventBridge::onControllerData(
    const std::vector<DIDEVICEOBJECTDATA>&& datas, uint32_t deviceType)
{
    for (const auto& data : datas)
    {
        printf("Joystick: %lu\n", data.dwOfs);
    }
}

HRY_NS_END
