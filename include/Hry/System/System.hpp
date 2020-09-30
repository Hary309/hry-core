/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

enum ResizeType
{
    Maximized = 0,
    Minimized,
    Restored
};

enum class ButtonState
{
    Pressed,
    Released
};

HRY_NS_END
