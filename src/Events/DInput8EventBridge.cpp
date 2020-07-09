#include "DInput8EventBridge.hpp"
#include "Events/EventBridgeBase.hpp"

#include <cstdio>

#include <dinput.h>

#include "Hooks/DInput8Hook.hpp"

#include "EventManager.hpp"
#include "Events/Event.hpp"
#include "System/Mouse.hpp"

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
            case DIMOFS_X:
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
            case DIMOFS_Y:
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
            case DIMOFS_Z:
            {
                MouseWheelEvent wheelEvent;
                wheelEvent.wheel = Mouse::Wheel::Horizontal;
                wheelEvent.delta = static_cast<short>(data.dwData / WHEEL_DELTA);

                Event event;
                event.type = Event::Type::MouseWheelScrolled;
                event.event = wheelEvent;
                _eventMgr.pushEvent(std::move(event));
            } break;

            // mouse buttons
            case DIMOFS_BUTTON0:
            {
                sendButtonEvent(data.dwData, Mouse::Button::Left);
            } break;
            case DIMOFS_BUTTON1:
            {
                sendButtonEvent(data.dwData, Mouse::Button::Right);
            } break;
            case DIMOFS_BUTTON2:
            {
                sendButtonEvent(data.dwData, Mouse::Button::Middle);
            } break;
            case DIMOFS_BUTTON3:
            {
                sendButtonEvent(data.dwData, Mouse::Button::XButton1);
            } break;
            case DIMOFS_BUTTON4:
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
