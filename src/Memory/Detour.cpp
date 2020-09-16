#include "Hry/Memory/Detour.hpp"

#include <MinHook.h>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

Detour::Detour(uintptr_t* target, uintptr_t* detour) : _target(target), _detour(detour)
{
}

Detour::Detour(uintptr_t target, uintptr_t detour) : Detour((uintptr_t*)target, (uintptr_t*)detour)
{
}

Detour::~Detour()
{
    disable();
    unhook();
}

Detour::Status Detour::hook()
{
    return static_cast<Detour::Status>(
        MH_CreateHook(static_cast<void*>(_target), _detour, reinterpret_cast<void**>(&_original)));
}

Detour::Status Detour::unhook()
{
    return static_cast<Detour::Status>(MH_RemoveHook(static_cast<void*>(_target)));
}

Detour::Status Detour::enable()
{
    return static_cast<Detour::Status>(MH_EnableHook(_target));
}

Detour::Status Detour::disable()
{
    return static_cast<Detour::Status>(MH_DisableHook(_target));
}

HRY_NS_END
