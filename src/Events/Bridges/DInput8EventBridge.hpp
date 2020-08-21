#pragma once

#include <vector>

#include "Hry/Math/Vec2.hpp"
#include "Hry/System/DeviceGUID.hpp"
#include "Hry/System/Mouse.hpp"

#include "Events/EventBridgeBase.hpp"

struct DIDEVICEOBJECTDATA;

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class DInput8EventBridge : public EventBridgeBase
{
private:
    Vec2i _mouseOffset{};

public:
    explicit DInput8EventBridge(EventManager& eventMgr);

private:
    void onMouseData(const std::vector<DIDEVICEOBJECTDATA>&&);
    void onJoystickData(const std::vector<DIDEVICEOBJECTDATA>&&, const DeviceGUID&& guid);
    void sendMouseButtonEvent(int pressData, Mouse::Button button);
};

HRY_NS_END
