#include "Hry/System//Mouse.hpp"

#include "Hooks/DInput8Hook.hpp"

namespace hry::system
{

void Mouse::DisableInGameMouse(bool disable) 
{
    hooks::DInput8Hook::disableInGameMouse = disable;
}

}
