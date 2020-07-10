#pragma once

#include <vector>

#include "EventBridgeBase.hpp"

#include "Hry/System/Mouse.hpp"


struct IDirectInputDevice8A;
struct DIDEVICEOBJECTDATA;

namespace hry::events
{

class DInput8EventBridge : public EventBridgeBase
{
private:
    int _mouseOffsetX;
    int _mouseOffsetY;

public:
    DInput8EventBridge(EventManager& eventMgr);

private:
    void onGetDeviceData(IDirectInputDevice8A*, const std::vector<DIDEVICEOBJECTDATA>&&);
    void sendButtonEvent(int pressData, system::Mouse::Button button);
};

}
