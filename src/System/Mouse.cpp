#include "Hry/System//Mouse.hpp"

#include "Hooks/DInput8Hook.hpp"

namespace hry
{
void Mouse::DisableInGameMouse(bool disable)
{
    DInput8Hook::disableInGameMouse = disable;
}

} // namespace hry
