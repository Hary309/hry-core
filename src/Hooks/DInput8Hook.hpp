#pragma once

#include <cstdint>
#include <vector>

#include <guiddef.h>

#include "Hry/System/Joystick.hpp"
#include "Hry/Utils/Delegate.hpp"

struct DIDEVICEOBJECTDATA;

HRY_NS_BEGIN

struct DInput8Hook
{
    using OnMouseData_t = Delegate<void(const std::vector<DIDEVICEOBJECTDATA>&&)>;
    using OnJoystickData_t = Delegate<void(const std::vector<DIDEVICEOBJECTDATA>&&, const GUID&)>;

    inline static OnMouseData_t OnMouseData;
    inline static OnJoystickData_t OnJoystickData;

    inline static bool DisableInGameMouse;

    static bool Install();
    static void Uninstall();
};

HRY_NS_END
