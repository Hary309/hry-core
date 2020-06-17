#pragma once
#include "Memory.hpp"

namespace hry::memory
{

template<typename T, typename Func>
Func HookVTable(T* target, size_t index, Func function)
{
    auto vtable = reinterpret_cast<uintptr_t*>(target);
    auto oFunc = reinterpret_cast<Func>(vtable[index]);

    WriteMemory(vtable + index, function, sizeof(uintptr_t*));

    return oFunc;
}

}
