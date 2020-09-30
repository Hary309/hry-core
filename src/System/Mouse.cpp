/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Hry/System//Mouse.hpp"

#include "Hry/Namespace.hpp"

#include "Hooks/DInput8Hook.hpp"

HRY_NS_BEGIN

void Mouse::DisableInGameMouse(bool disable)
{
    DInput8Hook::DisableInGameMouse = disable;
}

HRY_NS_END
