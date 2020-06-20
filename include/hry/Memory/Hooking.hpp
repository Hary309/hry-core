#pragma once
#include "Memory.hpp"

namespace hry::memory
{

template<typename Func>
Func HookVTableField(Func* vtableMethod, Func function)
{
    if (vtableMethod == nullptr || function == nullptr)
    {
        return nullptr;
    }

    Func original = *vtableMethod;

    WriteMemory(vtableMethod, function);

    return original;
}

}
