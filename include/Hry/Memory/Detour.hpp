#pragma once

#include <cstdint>
#include <type_traits>

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

    template<typename Function>
    Detour(Function target, Function detour)
        : _target(reinterpret_cast<uintptr_t*>(target)),
          _detour(reinterpret_cast<uintptr_t*>(detour))
    {
    }

    Detour(Detour&&) = delete;
    Detour(const Detour&) = delete;
    Detour& operator=(Detour&&) = delete;
    Detour& operator=(const Detour&) = delete;
    ~Detour();

    Status hook();
    Status enable();
    Status disable();
    Status unhook();

    // remove pointer from T (if exists) and than cast to pointer, it will prevent double pointer
    template<typename T, typename U = std::remove_pointer_t<T>>
    U* getOriginal()
    {
        return reinterpret_cast<U*>(_original);
    }
};

HRY_NS_END
