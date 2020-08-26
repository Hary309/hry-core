#pragma once

#include <vector>

#include <guiddef.h>

#include "Hry/Math/Vec2.hpp"
#include "Hry/System/Mouse.hpp"

#include "Events/EventProxyBase.hpp"

struct DIDEVICEOBJECTDATA;

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class DInput8EventProxy : public EventProxyBase
{
private:
    Vec2i _mouseOffset{};

public:
    explicit DInput8EventProxy(EventManager& eventMgr);

private:
    void onMouseData(const std::vector<DIDEVICEOBJECTDATA>&& events);
    void onJoystickData(const std::vector<DIDEVICEOBJECTDATA>&& events, const GUID& guid);
    void sendMouseButtonEvent(int pressData, Mouse::Button button);
};

HRY_NS_END