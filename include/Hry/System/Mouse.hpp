/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

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
        Button4,
        Button5,
        Button6,
        Button7,
        Button8
    };

    enum class Wheel
    {
        Vertical,
        Horizontal
    };

    static void DisableInGameMouse(bool disable);
};

HRY_NS_END