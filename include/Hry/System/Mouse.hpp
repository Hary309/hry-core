#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

struct Mouse
{
    enum class Button
    {
        Left,
        Right,
        Middle,
        XButton1,
        XButton2
    };

    enum class Wheel
    {
        Vertical,
        Horizontal
    };

    static void DisableInGameMouse(bool disable);
};

HRY_NS_END