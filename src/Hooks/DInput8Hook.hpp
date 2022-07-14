/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/System/Joystick.hpp"
#include "Hry/Utils/Delegate.hpp"

#include <guiddef.h>

#include <cstdint>
#include <vector>

struct DIDEVICEOBJECTDATA;

namespace hry
{
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
}
