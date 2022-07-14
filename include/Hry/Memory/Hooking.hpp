/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include "Memory.hpp"

namespace hry
{
/**
 * @brief Hooks vtable field
 *
 * @tparam Func Function type
 * @param vtableMethod Pointer to vtable field
 * @param function Pointer to function, which will override the \p vtableMethod
 * @return Pointer to original vtable method
 */
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
