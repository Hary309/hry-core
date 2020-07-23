#include "Hry/System//Mouse.hpp"

#include "Hry/Namespace.hpp"

#include "Hooks/DInput8Hook.hpp"

HRY_NS_BEGIN

void Mouse::DisableInGameMouse(bool disable)
{
    DInput8Hook::disableInGameMouse = disable;
}

HRY_NS_END
