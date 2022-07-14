/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Events/EventProxyBase.hpp"

#include "Hry/Math/Vec2.hpp"
#include "Hry/System/Joystick.hpp"
#include "Hry/System/Mouse.hpp"
#include "Hry/System/System.hpp"

#include <guiddef.h>
#include <rpc.h>

#include <unordered_map>
#include <vector>

struct DIDEVICEOBJECTDATA;

namespace std
{
template<>
struct hash<GUID>
{
    size_t operator()(const GUID& guid) const noexcept
    {
        const auto* p = reinterpret_cast<const std::uint64_t*>(&guid);
        std::hash<std::uint64_t> hash;
        return hash(p[0]) ^ hash(p[1]);
    }
};
}

namespace hry
{
class DInput8EventProxy : public EventProxyBase
{
    struct DPadStatus
    {
        bool left;
        bool right;
        bool up;
        bool down;
    };

private:
    std::unordered_map<GUID, DPadStatus> _dpadStatus;

public:
    explicit DInput8EventProxy(EventManager& eventMgr);

private:
    void onMouseData(const std::vector<DIDEVICEOBJECTDATA>&& events) noexcept;
    void sendMouseButtonEvent(int pressData, Mouse::Button button) noexcept;

    void onJoystickData(const std::vector<DIDEVICEOBJECTDATA>&& events, const GUID& guid) noexcept;
    void sendJoystickButtonEvent(GUID deviceGUID, Joystick::Button button, ButtonState buttonState);

    void sendJoystickDPadEvent(
        double value,
        GUID deviceGUID,
        bool& field1,
        bool& field2,
        Joystick::Button button1,
        Joystick::Button button2);
};
}
