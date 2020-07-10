#pragma once

#include <cstdint>
#include <MinHook.h>

namespace hry::memory
{

class Detour
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
    uintptr_t* _target;
    uintptr_t* _detour;
    uintptr_t* _original;

public:
    Detour(uintptr_t target, uintptr_t detour);
    Detour(uintptr_t* target, uintptr_t* detour);
    ~Detour();

    Status hook();
    Status enable();
    Status disable();

    template<typename T>
    T* get() { return reinterpret_cast<T>(_original); } 
};

}
