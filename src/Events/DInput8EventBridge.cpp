#include "DInput8EventBridge.hpp"

#include <cstdio>

#include <dinput.h>

#include "Hry/Events/Event.hpp"
#include "Hry/System/Mouse.hpp"

#include "EventManager.hpp"
#include "Events/EventBridgeBase.hpp"
#include "Hooks/DInput8Hook.hpp"

constexpr int DINPUT_X = offsetof(DIMOUSESTATE, lX);
constexpr int DINPUT_Y = offsetof(DIMOUSESTATE, lY);
constexpr int DINPUT_Z = offsetof(DIMOUSESTATE, lZ);
constexpr int DINPUT_BUTTON0 = (offsetof(DIMOUSESTATE, rgbButtons) + 0);
constexpr int DINPUT_BUTTON1 = (offsetof(DIMOUSESTATE, rgbButtons) + 1);
constexpr int DINPUT_BUTTON2 = (offsetof(DIMOUSESTATE, rgbButtons) + 2);
constexpr int DINPUT_BUTTON3 = (offsetof(DIMOUSESTATE, rgbButtons) + 3);
constexpr int DINPUT_BUTTON4 = (offsetof(DIMOUSESTATE, rgbButtons) + 4);

namespace hry::events
{

DInput8EventBridge::DInput8EventBridge(EventManager& eventMgr)
        : EventBridgeBase(eventMgr)
{
    hooks::DInput8Hook::OnGetDeviceData.connect<&DInput8EventBridge::onGetDeviceData>(this);
}

void DInput8EventBridge::onGetDeviceData(IDirectInputDevice8A* device, const std::vector<DIDEVICEOBJECTDATA>&& datas)
{
    using namespace system;

    for (auto& data : datas)
    {
        switch (data.dwOfs)
        {
            // mouse move x
            case DINPUT_X:
            {
                _mouseOffsetX = data.dwData;

                MouseMoveEvent moveEvent;
                moveEvent.offsetX = _mouseOffsetX;
                moveEvent.offsetY = _mouseOffsetY;

                Event event;
                event.type = Event::Type::MouseMoved;
                event.event = moveEvent;
                _eventMgr.pushEvent(std::move(event));
            } break;
            case DINPUT_Y:
            {
                _mouseOffsetY = data.dwData;

                MouseMoveEvent moveEvent;
                moveEvent.offsetX = _mouseOffsetX;
                moveEvent.offsetY = _mouseOffsetY;

                Event event;
                event.type = Event::Type::MouseMoved;
                event.event = moveEvent;
                _eventMgr.pushEvent(std::move(event));
            } break;

            // mouse move wheel
            case DINPUT_Z:
            {
                MouseWheelEvent wheelEvent;
                wheelEvent.wheel = Mouse::Wheel::Vertical;
                wheelEvent.delta = static_cast<short>(data.dwData) / WHEEL_DELTA;

                Event event;
                event.type = Event::Type::MouseWheelScrolled;
                event.event = wheelEvent;
                _eventMgr.pushEvent(std::move(event));
            } break;

            // mouse buttons
            case DINPUT_BUTTON0:
            {
                sendButtonEvent(data.dwData, Mouse::Button::Left);
            } break;
            case DINPUT_BUTTON1:
            {
                sendButtonEvent(data.dwData, Mouse::Button::Right);
            } break;
            case DINPUT_BUTTON2:
            {
                sendButtonEvent(data.dwData, Mouse::Button::Middle);
            } break;
            case DINPUT_BUTTON3:
            {
                sendButtonEvent(data.dwData, Mouse::Button::XButton1);
            } break;
            case DINPUT_BUTTON4:
            {
                sendButtonEvent(data.dwData, Mouse::Button::XButton2);
            } break;
        }
    }
}


void DInput8EventBridge::sendButtonEvent(int pressData, system::Mouse::Button button)
{
    events::MouseButtonEvent mouseButtonEvent;
    mouseButtonEvent.button = button;

    events::Event event;
    event.event = mouseButtonEvent;
    event.type = pressData == 0x80 ? Event::Type::MouseButtonPressed : Event::Type::MouseButtonReleased;

    _eventMgr.pushEvent(std::move(event));
}

}
