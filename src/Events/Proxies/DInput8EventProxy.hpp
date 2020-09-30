/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

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
    void onMouseData(const std::vector<DIDEVICEOBJECTDATA>&& events) noexcept;
    void onJoystickData(const std::vector<DIDEVICEOBJECTDATA>&& events, const GUID& guid) noexcept;
    void sendMouseButtonEvent(int pressData, Mouse::Button button) noexcept;
};

HRY_NS_END
