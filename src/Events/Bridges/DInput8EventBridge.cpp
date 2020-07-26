#include "DInput8EventBridge.hpp"

#include <cstdio>

#include <dinput.h>

#include "Hry/Events/Event.hpp"
#include "Hry/System/Mouse.hpp"

#include "Events/EventManager.hpp"
#include "Hooks/DInput8Hook.hpp"

constexpr int DINPUT_X = offsetof(DIMOUSESTATE, lX);
constexpr int DINPUT_Y = offsetof(DIMOUSESTATE, lY);
constexpr int DINPUT_Z = offsetof(DIMOUSESTATE, lZ);
constexpr int DINPUT_BUTTON0 = (offsetof(DIMOUSESTATE, rgbButtons) + 0);
constexpr int DINPUT_BUTTON1 = (offsetof(DIMOUSESTATE, rgbButtons) + 1);
constexpr int DINPUT_BUTTON2 = (offsetof(DIMOUSESTATE, rgbButtons) + 2);
constexpr int DINPUT_BUTTON3 = (offsetof(DIMOUSESTATE, rgbButtons) + 3);
constexpr int DINPUT_BUTTON4 = (offsetof(DIMOUSESTATE, rgbButtons) + 4);

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

DInput8EventBridge::DInput8EventBridge(EventManager& eventMgr) : EventBridgeBase(eventMgr)
{
    DInput8Hook::OnGetDeviceData.connect<&DInput8EventBridge::onGetDeviceData>(this);
}

void DInput8EventBridge::onGetDeviceData(
    IDirectInputDevice8A*, const std::vector<DIDEVICEOBJECTDATA>&& datas)
{
    for (auto& data : datas)
    {
        switch (data.dwOfs)
        {
            // mouse move x
            case DINPUT_X:
            {
                _mouseOffset.x = data.dwData;

                MouseMoveEvent moveEvent;
                moveEvent.offset = _mouseOffset;

                _eventMgr.mouseMoveSignal.call(std::move(moveEvent));
            }
            break;
            case DINPUT_Y:
            {
                _mouseOffset.y = data.dwData;

                MouseMoveEvent moveEvent;
                moveEvent.offset = _mouseOffset;

                _eventMgr.mouseMoveSignal.call(std::move(moveEvent));
            }
            break;

            // mouse move wheel
            case DINPUT_Z:
            {
                MouseWheelEvent wheelEvent;
                wheelEvent.wheel = Mouse::Wheel::Vertical;
                wheelEvent.delta = static_cast<short>(data.dwData) / WHEEL_DELTA;

                _eventMgr.mouseWheelScrollSignal.call(std::move(wheelEvent));
            }
            break;

            // mouse buttons
            case DINPUT_BUTTON0:
            {
                sendButtonEvent(data.dwData, Mouse::Button::Left);
            }
            break;
            case DINPUT_BUTTON1:
            {
                sendButtonEvent(data.dwData, Mouse::Button::Right);
            }
            break;
            case DINPUT_BUTTON2:
            {
                sendButtonEvent(data.dwData, Mouse::Button::Middle);
            }
            break;
            case DINPUT_BUTTON3:
            {
                sendButtonEvent(data.dwData, Mouse::Button::XButton1);
            }
            break;
            case DINPUT_BUTTON4:
            {
                sendButtonEvent(data.dwData, Mouse::Button::XButton2);
            }
            break;
        }
    }
}

void DInput8EventBridge::sendButtonEvent(int pressData, Mouse::Button button)
{
    MouseButtonEvent mouseButtonEvent;
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

HRY_NS_END
