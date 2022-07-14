/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Hry/System//Mouse.hpp"


#include "Hooks/DInput8Hook.hpp"

namespace hry
{
void Mouse::DisableInGameMouse(bool disable)
{
    DInput8Hook::DisableInGameMouse = disable;
}
}
