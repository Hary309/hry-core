#pragma once

#include "Hry/Namespace.hpp"
#include "Hry/Utils/Delegate.hpp"

HRY_NS_BEGIN

struct XInputHook
{
    inline static Delegate<void()> OnJoystickData;

    static bool Install();
    static void Uninstall();
};

HRY_NS_END
