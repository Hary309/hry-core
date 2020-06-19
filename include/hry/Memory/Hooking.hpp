#pragma once
#include "Memory.hpp"

namespace hry::memory
{

template<typename Func>
Func HookVTableField(Func* vtableMethod, Func function)
{
    Func original = *vtableMethod;

    WriteMemory(vtableMethod, function);

    return original;
}

}
