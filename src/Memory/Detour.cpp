#include "Memory/Detour.hpp"
#include "MinHook.h"
#include <vadefs.h>

namespace hry::memory
{

Detour::Detour(uintptr_t* target, uintptr_t* detour)
    : _target(target), _detour(detour)
{
}

Detour::Detour(uintptr_t target, uintptr_t detour) 
    : Detour((uintptr_t*)target, (uintptr_t*)detour)
{
}

Detour::~Detour() 
{
    disable();
}

Detour::Status Detour::hook() 
{
    return static_cast<Detour::Status>(MH_CreateHook(
            static_cast<void*>(_target),
            _detour,
            reinterpret_cast<void**>(_original))
        );
}

Detour::Status Detour::enable() 
{
    return static_cast<Detour::Status>(MH_EnableHook(_target));
}

Detour::Status Detour::disable()
{
    return static_cast<Detour::Status>(MH_DisableHook(_target));
}

}
