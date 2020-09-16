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
    remove();
}

Detour::Status Detour::create()
{
    return static_cast<Detour::Status>(
        MH_CreateHook(static_cast<void*>(_target), _detour, reinterpret_cast<void**>(&_original)));
}

Detour::Status Detour::remove()
{
    auto status = static_cast<Detour::Status>(MH_RemoveHook(static_cast<void*>(_target)));

    if (status == Detour::Status::Ok)
    {
        _target = nullptr;
        _detour = nullptr;
        _original = nullptr;
    }

    return status;
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
