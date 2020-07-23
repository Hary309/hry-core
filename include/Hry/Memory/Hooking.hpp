#pragma once
#include "Hry/Namespace.hpp"

#include "Memory.hpp"

HRY_NS_BEGIN

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

HRY_NS_END
