#pragma once

#include <cstdint>

#include "Hry/Export.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

class HRY_API Detour
{
public:
    enum class Status
    {
        Unknown = -1,
        Ok = 0,
        AlreadyInitialized,
        NotInitialized,
        AlreadyCreated,
        NotCreated,
        AlreadyEnabled,
        Disabled,
        NotExecutable,
        UnsupportedFunction,
        MemoryAlloc,
        MemoryProtect,
        ModuleNotFound,
        FunctionNotFound
    };

private:
    uintptr_t* _target{};
    uintptr_t* _detour{};
    uintptr_t* _original{};

public:
    Detour(uintptr_t target, uintptr_t detour);
    Detour(uintptr_t* target, uintptr_t* detour);
    Detour(Detour&&) = default;
    Detour(const Detour&) = default;
    Detour& operator=(Detour&&) = default;
    Detour& operator=(const Detour&) = default;
    ~Detour();

    Status hook();
    Status enable();
    Status disable();

    template<typename T>
    T* get()
    {
        return static_cast<T*>(_original);
    }
};

HRY_NS_END
