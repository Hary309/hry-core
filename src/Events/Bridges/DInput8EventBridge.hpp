#pragma once

#include <vector>

#include "Hry/Math/Vec2.hpp"
#include "Hry/System/Mouse.hpp"

#include "Events/EventBridgeBase.hpp"

struct IDirectInputDevice8A;
struct DIDEVICEOBJECTDATA;

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class DInput8EventBridge : public EventBridgeBase
{
private:
    Vec2i _mouseOffset;

public:
    DInput8EventBridge(EventManager& eventMgr);

private:
    void onGetDeviceData(IDirectInputDevice8A*, const std::vector<DIDEVICEOBJECTDATA>&&);
    void sendButtonEvent(int pressData, Mouse::Button button);
};

HRY_NS_END
