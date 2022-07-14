/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Utils/Delegate.hpp"

struct _XINPUT_STATE;

namespace hry
{
struct XInputHook
{
    inline static Delegate<void(uint32_t, uint32_t, _XINPUT_STATE&)> OnJoystickData;

    static bool Install();
    static void Uninstall();
};
}
