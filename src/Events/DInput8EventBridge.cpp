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

    for (auto& data : datas)
    {
        switch (data.dwOfs)
        {
            // mouse move x
            case DIMOFS_X:
                // TODO: send mouse pos
            break;
            // mouse move y
            case DIMOFS_Y:
                // TODO: send mouse pos
            break;
            // mouse move wheel
            case DIMOFS_Z:
                // TODO: send mouse wheel
            break;
            
            // mouse buttons
            case DIMOFS_BUTTON0:
            {
                sendButtonEvent(data.dwData, system::Mouse::Button::Left);
            } break;
            case DIMOFS_BUTTON1:
            {
                sendButtonEvent(data.dwData, system::Mouse::Button::Right);
            } break;
            case DIMOFS_BUTTON2:
            {
                sendButtonEvent(data.dwData, system::Mouse::Button::Middle);
            } break;
            case DIMOFS_BUTTON3:
            {
                sendButtonEvent(data.dwData, system::Mouse::Button::XButton1);
            } break;
            case DIMOFS_BUTTON4:
            {
                sendButtonEvent(data.dwData, system::Mouse::Button::XButton2);
            } break;
        }
    }
}


void DInput8EventBridge::sendButtonEvent(int pressData, system::Mouse::Button button) 
{
    events::MouseButtonEvent mouseButtonEvent;
    mouseButtonEvent.button = button;

    // TODO: fill with correct value
    mouseButtonEvent.x = 0;
    mouseButtonEvent.y = 0;

    events::Event event;
    event.event = mouseButtonEvent;
    event.type = pressData == 0x80 ? Event::Type::MouseButtonPressed : Event::Type::MouseButtonReleased;

    _eventMgr.pushEvent(std::move(event));
}

}
