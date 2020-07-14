#pragma once

#include <vector>

#include "EventBridgeBase.hpp"

#include "Hry/Math/Vec2.hpp"
#include "Hry/System/Mouse.hpp"


struct IDirectInputDevice8A;
struct DIDEVICEOBJECTDATA;

namespace hry::events
{

class DInput8EventBridge : public EventBridgeBase
{
private:
    math::Vec2i _mouseOffset;

public:
    DInput8EventBridge(EventManager& eventMgr);

private:
    void onGetDeviceData(IDirectInputDevice8A*, const std::vector<DIDEVICEOBJECTDATA>&&);
    void sendButtonEvent(int pressData, system::Mouse::Button button);
};

}
