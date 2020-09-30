/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <cstdint>
#include <memory>

#include <Windows.h>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

template<typename T, typename U>
void WriteMemory(T* target, U data, const size_t size)
{
    DWORD oldProtect = 0;
    DWORD newProtect = 0;

    VirtualProtect(reinterpret_cast<void*>(target), size, PAGE_READWRITE, &oldProtect);

    memcpy(reinterpret_cast<void*>(target), &data, size);

    VirtualProtect(reinterpret_cast<void*>(target), size, oldProtect, &newProtect);
}

template<typename T, typename U>
void WriteMemory(T* target, U data)
{
    WriteMemory(target, data, sizeof(U));
}

inline uintptr_t GetBaseAddress()
{
    return reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr)) - 0x140000000;
}

HRY_NS_END
